#pragma once

#include <string>

class XmlParser;

class XmlInfo
{
public:
    using Parser = XmlParser;

    enum class Type
    {
        DocumentBegin,
        DocumentEnd,
        ElementBegin,
        ElementEnd,
        Attribute,
        Characters,
        CData,
        Comment,
        ProcessingInstruction
    };

    Type type = Type::Characters;
    std::string name;
    std::string value;
};
