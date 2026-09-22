#include "JsonParser.h"

#include <charconv>
#include <cctype>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <system_error>


namespace
{
    bool IsDigit(char c)
    {
        return c >= '0' && c <= '9';
    }
}

bool JsonParser::Parse(std::istream& input)
{
    m_data.assign(std::istreambuf_iterator<char>(input),
                  std::istreambuf_iterator<char>());

    m_pos = 0;

    if (!OnEvent({ JsonInfo::Type::DocumentBegin, {}, {} }))
        return false;

    SkipWhitespace();

    if (!ParseValue())
        return false;

    SkipWhitespace();

    if (!End())
        return false;

    return OnEvent({ JsonInfo::Type::DocumentEnd, {}, {} });
}

bool JsonParser::ParseValue(const std::string& name)
{
    SkipWhitespace();

    if (End())
        return false;

    switch (Current())
    {
    case '{':
        if (!Emit(JsonInfo::Type::ObjectBegin, name))
            return false;
        if (!ParseObject())
            return false;
        return Emit(JsonInfo::Type::ObjectEnd);

    case '[':
        if (!Emit(JsonInfo::Type::ArrayBegin, name))
            return false;
        if (!ParseArray())
            return false;
        return Emit(JsonInfo::Type::ArrayEnd);

    case '"':
    {
        std::string value;
        if (!ParseString(value))
            return false;
        return Emit(JsonInfo::Type::String, name, value);
    }

    case 't':
    case 'f':
    {
        std::string value;
        if (!ParseLiteral(value))
            return false;
        return Emit(JsonInfo::Type::Boolean, name, value);
    }

    case 'n':
    {
        std::string value;
        if (!ParseLiteral(value))
            return false;
        if (value != "null")
            return false;
        return Emit(JsonInfo::Type::Null, name);
    }

    default:
        if (Current() == '-' || IsDigit(Current()))
        {
            std::string value;
            if (!ParseNumber(value))
                return false;
            return Emit(JsonInfo::Type::Number, name, value);
        }
        return false;
    }
}

bool JsonParser::ParseObject()
{
    if (!Consume('{'))
        return false;

    SkipWhitespace();

    if (Consume('}'))
        return true;

    while (!End())
    {
        SkipWhitespace();

        if (Current() != '"')
            return false;

        std::string name;
        if (!ParseString(name))
            return false;

        SkipWhitespace();

        if (!Consume(':'))
            return false;

        if (!ParseValue(name))
            return false;

        SkipWhitespace();

        if (Consume('}'))
            return true;

        if (!Consume(','))
            return false;
    }

    return false;
}

bool JsonParser::ParseArray()
{
    if (!Consume('['))
        return false;

    SkipWhitespace();

    if (Consume(']'))
        return true;

    while (!End())
    {
        if (!ParseValue())
            return false;

        SkipWhitespace();

        if (Consume(']'))
            return true;

        if (!Consume(','))
            return false;
    }

    return false;
}

bool JsonParser::ParseString(std::string& result)
{
    result.clear();

    if (!Consume('"'))
        return false;

    while (!End())
    {
        char c = Current();
        ++m_pos;

        if (c == '"')
            return true;

        if (static_cast<unsigned char>(c) < 0x20)
            return false;

        if (c != '\\')
        {
            result += c;
            continue;
        }

        if (End())
            return false;

        char escaped = Current();
        ++m_pos;

        switch (escaped)
        {
        case '"':  result += '"';  break;
        case '\\': result += '\\'; break;
        case '/':  result += '/';  break;
        case 'b':  result += '\b'; break;
        case 'f':  result += '\f'; break;
        case 'n':  result += '\n'; break;
        case 'r':  result += '\r'; break;
        case 't':  result += '\t'; break;

        case 'u':
        {
            if (m_pos + 4 > m_data.size())
                return false;

            unsigned codepoint = 0;

            for (int i = 0; i < 4; ++i)
            {
                char h = m_data[m_pos++];
                if (!IsHexDigit(h))
                    return false;

                codepoint = (codepoint << 4) | HexValue(h);
            }

            // Handle a UTF-16 surrogate pair.
            if (codepoint >= 0xD800 && codepoint <= 0xDBFF)
            {
                if (m_pos + 6 <= m_data.size() &&
                    m_data[m_pos] == '\\' &&
                    m_data[m_pos + 1] == 'u')
                {
                    m_pos += 2;

                    unsigned low = 0;
                    for (int i = 0; i < 4; ++i)
                    {
                        char h = m_data[m_pos++];
                        if (!IsHexDigit(h))
                            return false;

                        low = (low << 4) | HexValue(h);
                    }

                    if (low < 0xDC00 || low > 0xDFFF)
                        return false;

                    codepoint = 0x10000 +
                        ((codepoint - 0xD800) << 10) +
                        (low - 0xDC00);
                }
                else
                {
                    return false;
                }
            }
            else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF)
            {
                return false;
            }

            AppendUtf8(result, codepoint);
            break;
        }

        default:
            return false;
        }
    }

    return false;
}

bool JsonParser::ParseNumber(std::string& result)
{
    const std::size_t start = m_pos;

    if (Consume('-'))
    {
        if (End())
            return false;
    }

    if (Consume('0'))
    {
        if (!End() && IsDigit(Current()))
            return false;
    }
    else
    {
        if (End() || !IsDigit(Current()))
            return false;

        while (!End() && IsDigit(Current()))
            ++m_pos;
    }

    if (Consume('.'))
    {
        if (End() || !IsDigit(Current()))
            return false;

        while (!End() && IsDigit(Current()))
            ++m_pos;
    }

    if (!End() && (Current() == 'e' || Current() == 'E'))
    {
        ++m_pos;

        if (!End() && (Current() == '+' || Current() == '-'))
            ++m_pos;

        if (End() || !IsDigit(Current()))
            return false;

        while (!End() && IsDigit(Current()))
            ++m_pos;
    }

    result = m_data.substr(start, m_pos - start);
    return true;
}

bool JsonParser::ParseLiteral(std::string& result)
{
    const std::size_t start = m_pos;

    while (!End() && std::isalpha(static_cast<unsigned char>(Current())))
        ++m_pos;

    result = m_data.substr(start, m_pos - start);

    return result == "true" ||
           result == "false" ||
           result == "null";
}

void JsonParser::SkipWhitespace()
{
    while (!End() &&
           (Current() == ' ' ||
            Current() == '\t' ||
            Current() == '\r' ||
            Current() == '\n'))
    {
        ++m_pos;
    }
}

bool JsonParser::End() const
{
    return m_pos >= m_data.size();
}

char JsonParser::Current() const
{
    return End() ? '\0' : m_data[m_pos];
}

bool JsonParser::Consume(char c)
{
    if (!End() && Current() == c)
    {
        ++m_pos;
        return true;
    }

    return false;
}

bool JsonParser::Emit(JsonInfo::Type type,
                       const std::string& name,
                       const std::string& value)
{
    JsonInfo info;
    info.type = type;
    info.name = name;
    info.value = value;

    return ReadField(info);
}

bool JsonParser::IsNumberChar(char c)
{
    return IsDigit(c) ||
           c == '-' ||
           c == '+' ||
           c == '.' ||
           c == 'e' ||
           c == 'E';
}

bool JsonParser::IsHexDigit(char c)
{
    return IsDigit(c) ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

unsigned JsonParser::HexValue(char c)
{
    if (c >= '0' && c <= '9')
        return static_cast<unsigned>(c - '0');

    if (c >= 'a' && c <= 'f')
        return static_cast<unsigned>(c - 'a' + 10);

    return static_cast<unsigned>(c - 'A' + 10);
}

void JsonParser::AppendUtf8(std::string& out, unsigned cp)
{
    if (cp <= 0x7F)
    {
        out.push_back(static_cast<char>(cp));
    }
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
    else if (cp <= 0x10FFFF)
    {
        out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
    }
}

