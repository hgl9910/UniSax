#pragma once

#include "XmlInfo.h"
#include "BasicSaxParser.h"

#include <cstddef>
#include <istream>
#include <string>

class XmlParser : public BasicSaxParser<XmlInfo>
{
public:
    using BasicSaxParser<XmlInfo>::BasicSaxParser;

    bool Parse(std::istream& input);

private:
    std::string m_data;
    std::size_t m_pos = 0;

    bool ParseDocument();
    bool ParseElement();
    bool ParseText();
    bool ParseComment();
    bool ParseCData();
    bool ParseProcessingInstruction();
    bool SkipDoctype();
    bool ParseName(std::string& name);
    bool ParseQuotedValue(std::string& value);
    bool DecodeEntities(const std::string& source, std::string& result);
    bool Emit(XmlInfo::Type type, const std::string& name = {}, const std::string& value = {});
    void SkipWhitespace();
    bool StartsWith(const char* text) const;
    bool End() const;
    char Current() const;
    bool Consume(char c);
    bool Fail();
};
