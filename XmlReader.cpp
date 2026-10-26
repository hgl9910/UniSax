#include "XmlReader.h"

bool XmlReader::OnEvent(const XmlInfo& info)
{
    switch (info.type)
    {
    case XmlInfo::Type::DocumentBegin: return OnDocumentBegin();
    case XmlInfo::Type::DocumentEnd: return OnDocumentEnd();
    case XmlInfo::Type::ElementBegin: return OnElementBegin(info.name);
    case XmlInfo::Type::ElementEnd: return OnElementEnd(info.name);
    case XmlInfo::Type::Attribute: return OnAttribute(info.name, info.value);
    case XmlInfo::Type::Characters: return OnCharacters(info.value);
    case XmlInfo::Type::CData: return OnCData(info.value);
    case XmlInfo::Type::Comment: return OnComment(info.value);
    case XmlInfo::Type::ProcessingInstruction: return OnProcessingInstruction(info.name, info.value);
    default: return false;
    }
}
