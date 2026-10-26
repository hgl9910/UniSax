#pragma once

#include "BasicReader.h"
#include "XmlInfo.h"
#include "XmlParser.h"

class XmlReader : public BasicReader<XmlInfo>
{
protected:
    bool OnEvent(const XmlInfo& info) override;

    virtual bool OnDocumentBegin() { return true; }
    virtual bool OnDocumentEnd() { return true; }
    virtual bool OnElementBegin(const std::string&) { return true; }
    virtual bool OnElementEnd(const std::string&) { return true; }
    virtual bool OnAttribute(const std::string&, const std::string&) { return true; }
    virtual bool OnCharacters(const std::string&) { return true; }
    virtual bool OnCData(const std::string&) { return true; }
    virtual bool OnComment(const std::string&) { return true; }
    virtual bool OnProcessingInstruction(const std::string&, const std::string&) { return true; }
};
