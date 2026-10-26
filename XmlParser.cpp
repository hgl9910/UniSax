#include "XmlParser.h"

#include <iterator>
#include <string_view>

namespace
{
    bool IsNameDelimiter(char c)
    {
        return c == '\0' || c == ' ' || c == '\t' || c == '\r' || c == '\n' ||
               c == '/' || c == '>' || c == '=' || c == '?';
    }

    void AppendUtf8(std::string& out, unsigned long cp)
    {
        if (cp <= 0x7F)
            out.push_back(static_cast<char>(cp));
        else if (cp <= 0x7FF)
        {
            out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF)
        {
            out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else
        {
            out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }

    int HexDigit(char c)
    {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'f') return c - 'a' + 10;
        if (c >= 'A' && c <= 'F') return c - 'A' + 10;
        return -1;
    }
}

bool XmlParser::Parse(std::istream& input)
{
    m_data.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    m_pos = 0;

    // UTF-8 BOM, if present.
    if (m_data.size() >= 3 &&
        static_cast<unsigned char>(m_data[0]) == 0xEF &&
        static_cast<unsigned char>(m_data[1]) == 0xBB &&
        static_cast<unsigned char>(m_data[2]) == 0xBF)
        m_pos = 3;

    return ParseDocument();
}

bool XmlParser::ParseDocument()
{
    if (!Emit(XmlInfo::Type::DocumentBegin)) return false;

    while (!End())
    {
        if (StartsWith("<!--"))
        {
            if (!ParseComment()) return false;
        }
        else if (StartsWith("<?"))
        {
            if (!ParseProcessingInstruction()) return false;
        }
        else if (StartsWith("<!DOCTYPE") || StartsWith("<!doctype"))
        {
            if (!SkipDoctype()) return false;
        }
        else if (StartsWith("<![CDATA["))
        {
            // CDATA is only valid inside an element.
            return Fail();
        }
        else if (Current() == '<')
        {
            if (!ParseElement()) return false;
        }
        else
        {
            // Outside the document element, only whitespace is legal.
            if (Current() != ' ' && Current() != '\t' && Current() != '\r' && Current() != '\n')
                return Fail();
            ++m_pos;
        }
    }

    return Emit(XmlInfo::Type::DocumentEnd);
}

bool XmlParser::ParseElement()
{
    if (!Consume('<') || Current() == '/' || Current() == '!' || Current() == '?') return Fail();

    std::string elementName;
    if (!ParseName(elementName)) return Fail();
    if (!Emit(XmlInfo::Type::ElementBegin, elementName)) return false;

    for (;;)
    {
        SkipWhitespace();
        if (Consume('/'))
        {
            if (!Consume('>')) return Fail();
            return Emit(XmlInfo::Type::ElementEnd, elementName);
        }
        if (Consume('>')) break;
        if (End()) return Fail();

        std::string attributeName;
        std::string attributeValue;
        if (!ParseName(attributeName)) return Fail();
        SkipWhitespace();
        if (!Consume('=')) return Fail();
        SkipWhitespace();
        if (!ParseQuotedValue(attributeValue)) return Fail();
        if (!Emit(XmlInfo::Type::Attribute, attributeName, attributeValue)) return false;
    }

    for (;;)
    {
        if (End()) return Fail();
        if (StartsWith("</"))
        {
            m_pos += 2;
            std::string closeName;
            if (!ParseName(closeName) || closeName != elementName) return Fail();
            SkipWhitespace();
            if (!Consume('>')) return Fail();
            return Emit(XmlInfo::Type::ElementEnd, elementName);
        }
        if (StartsWith("<!--"))
        {
            if (!ParseComment()) return false;
        }
        else if (StartsWith("<![CDATA["))
        {
            if (!ParseCData()) return false;
        }
        else if (StartsWith("<?"))
        {
            if (!ParseProcessingInstruction()) return false;
        }
        else if (StartsWith("<!DOCTYPE") || StartsWith("<!doctype"))
        {
            return Fail();
        }
        else if (Current() == '<')
        {
            if (!ParseElement()) return false;
        }
        else
        {
            if (!ParseText()) return false;
        }
    }
}

bool XmlParser::ParseText()
{
    const std::size_t start = m_pos;
    while (!End() && Current() != '<') ++m_pos;
    if (m_pos == start) return true;

    std::string decoded;
    if (!DecodeEntities(m_data.substr(start, m_pos - start), decoded)) return Fail();
    return decoded.empty() ? true : Emit(XmlInfo::Type::Characters, {}, decoded);
}

bool XmlParser::ParseComment()
{
    m_pos += 4; // <!--
    const auto end = m_data.find("-->", m_pos);
    if (end == std::string::npos) return Fail();
    const std::string value = m_data.substr(m_pos, end - m_pos);
    if (value.find("--") != std::string::npos) return Fail();
    m_pos = end + 3;
    return Emit(XmlInfo::Type::Comment, {}, value);
}

bool XmlParser::ParseCData()
{
    m_pos += 9; // <![CDATA[
    const auto end = m_data.find("]]>", m_pos);
    if (end == std::string::npos) return Fail();
    const std::string value = m_data.substr(m_pos, end - m_pos);
    m_pos = end + 3;
    return Emit(XmlInfo::Type::CData, {}, value);
}

bool XmlParser::ParseProcessingInstruction()
{
    m_pos += 2; // <?
    std::string target;
    if (!ParseName(target)) return Fail();
    const auto end = m_data.find("?>", m_pos);
    if (end == std::string::npos) return Fail();
    std::size_t valueStart = m_pos;
    while (valueStart < end && (m_data[valueStart] == ' ' || m_data[valueStart] == '\t' ||
           m_data[valueStart] == '\r' || m_data[valueStart] == '\n')) ++valueStart;
    const std::string value = m_data.substr(valueStart, end - valueStart);
    m_pos = end + 2;
    return Emit(XmlInfo::Type::ProcessingInstruction, target, value);
}

bool XmlParser::SkipDoctype()
{
    // The SVD file may declare an external schema/DTD. We skip the declaration,
    // respecting quoted strings and an optional internal subset; we do not load external entities.
    bool inQuote = false;
    char quote = '\0';
    int subsetDepth = 0;
    while (!End())
    {
        const char c = Current();
        ++m_pos;
        if (inQuote)
        {
            if (c == quote) inQuote = false;
            continue;
        }
        if (c == '\'' || c == '"')
        {
            inQuote = true;
            quote = c;
        }
        else if (c == '[') ++subsetDepth;
        else if (c == ']')
        {
            if (subsetDepth == 0) return Fail();
            --subsetDepth;
        }
        else if (c == '>' && subsetDepth == 0) return true;
    }
    return Fail();
}

bool XmlParser::ParseName(std::string& name)
{
    const std::size_t start = m_pos;
    while (!End() && !IsNameDelimiter(Current())) ++m_pos;
    if (m_pos == start) return false;
    name.assign(m_data, start, m_pos - start);
    return true;
}

bool XmlParser::ParseQuotedValue(std::string& value)
{
    if (End() || (Current() != '\'' && Current() != '"')) return false;
    const char quote = Current();
    ++m_pos;
    const std::size_t start = m_pos;
    while (!End() && Current() != quote)
    {
        if (Current() == '<') return false;
        ++m_pos;
    }
    if (End()) return false;
    const std::string source = m_data.substr(start, m_pos - start);
    ++m_pos;
    return DecodeEntities(source, value);
}

bool XmlParser::DecodeEntities(const std::string& source, std::string& result)
{
    result.clear();
    for (std::size_t i = 0; i < source.size(); ++i)
    {
        if (source[i] != '&')
        {
            result.push_back(source[i]);
            continue;
        }
        const auto semi = source.find(';', i + 1);
        if (semi == std::string::npos) return false;
        const std::string entity = source.substr(i + 1, semi - i - 1);
        if (entity == "amp") result.push_back('&');
        else if (entity == "lt") result.push_back('<');
        else if (entity == "gt") result.push_back('>');
        else if (entity == "quot") result.push_back('"');
        else if (entity == "apos") result.push_back('\'');
        else if (!entity.empty() && entity[0] == '#')
        {
            unsigned long cp = 0;
            std::size_t j = 1;
            int base = 10;
            if (j < entity.size() && (entity[j] == 'x' || entity[j] == 'X'))
            {
                base = 16;
                ++j;
            }
            if (j == entity.size()) return false;
            for (; j < entity.size(); ++j)
            {
                int digit = base == 16 ? HexDigit(entity[j]) :
                    ((entity[j] >= '0' && entity[j] <= '9') ? entity[j] - '0' : -1);
                if (digit < 0 || digit >= base || cp > (0x10FFFFUL - static_cast<unsigned long>(digit)) / base)
                    return false;
                cp = cp * base + static_cast<unsigned long>(digit);
            }
            if (cp == 0 || cp > 0x10FFFF || (cp >= 0xD800 && cp <= 0xDFFF)) return false;
            AppendUtf8(result, cp);
        }
        else return false; // Unknown/external entities are intentionally not expanded.
        i = semi;
    }
    return true;
}

bool XmlParser::Emit(XmlInfo::Type type, const std::string& name, const std::string& value)
{
    return OnEvent(XmlInfo{ type, name, value });
}

void XmlParser::SkipWhitespace()
{
    while (!End() && (Current() == ' ' || Current() == '\t' || Current() == '\r' || Current() == '\n')) ++m_pos;
}

bool XmlParser::StartsWith(const char* text) const
{
    const std::string_view needle(text);
    return m_pos + needle.size() <= m_data.size() &&
           std::string_view(m_data).substr(m_pos, needle.size()) == needle;
}

bool XmlParser::End() const { return m_pos >= m_data.size(); }
char XmlParser::Current() const { return End() ? '\0' : m_data[m_pos]; }
bool XmlParser::Consume(char c)
{
    if (End() || Current() != c) return false;
    ++m_pos;
    return true;
}
bool XmlParser::Fail() { return false; }
