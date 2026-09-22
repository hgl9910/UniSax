#include "JsonReader.h"

bool JsonReader::OnEvent(const JsonInfo& info)
{
    switch (info.type)
    {
    case JsonInfo::Type::DocumentBegin:
        return OnDocumentBegin();

    case JsonInfo::Type::DocumentEnd:
        return OnDocumentEnd();

    case JsonInfo::Type::ObjectBegin:
        return OnObjectBegin(info.name);

    case JsonInfo::Type::ObjectEnd:
        return OnObjectEnd();

    case JsonInfo::Type::ArrayBegin:
        return OnArrayBegin(info.name);

    case JsonInfo::Type::ArrayEnd:
        return OnArrayEnd();

    case JsonInfo::Type::String:
        return OnString(info.name, info.value);

    case JsonInfo::Type::Number:
        return OnNumber(info.name, info.value);

    case JsonInfo::Type::Boolean:
        return OnBoolean(info.name, info.value == "true");

    case JsonInfo::Type::Null:
        return OnNull(info.name);

    default:
        return false;
    }
}