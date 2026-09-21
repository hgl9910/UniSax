#pragma once

#include "JsonInfo.h"
#include "BasicSaxParser.h"

#include <istream>
#include <string>

class JsonParser : public BasicSaxParser<JsonInfo>
{
public:
    using BasicSaxParser<JsonInfo>::BasicSaxParser;

    bool Parse(std::istream& input);

private:
    std::string m_data;
    std::size_t m_pos = 0;

    bool ParseValue(const std::string& name = {});
    bool ParseObject();
    bool ParseArray();
    bool ParseString(std::string& result);
    bool ParseNumber(std::string& result);
    bool ParseLiteral(std::string& result);

    void SkipWhitespace();
    bool End() const;
    char Current() const;
    bool Consume(char c);

    bool Emit(JsonInfo::Type type,
              const std::string& name = {},
              const std::string& value = {});

    static bool IsNumberChar(char c);
    static bool IsHexDigit(char c);
    static unsigned HexValue(char c);
    static void AppendUtf8(std::string& out, unsigned codepoint);
};