#pragma once

#include "BasicReader.h"
#include "JsonInfo.h"

#include <string>

class JsonReader : public BasicReader<JsonInfo>
{
protected:
    bool OnEvent(const JsonInfo& info) override;

    virtual bool OnDocumentBegin() { return true; }
    virtual bool OnDocumentEnd()   { return true; }

    virtual bool OnObjectBegin(const std::string& name) { return true; }
    virtual bool OnObjectEnd()                          { return true; }

    virtual bool OnArrayBegin(const std::string& name)  { return true; }
    virtual bool OnArrayEnd()                           { return true; }

    virtual bool OnString(const std::string& name,
                          const std::string& value)     { return true; }

    virtual bool OnNumber(const std::string& name,
                          const std::string& value)     { return true; }

    virtual bool OnBoolean(const std::string& name,
                           bool value)                  { return true; }

    virtual bool OnNull(const std::string& name)        { return true; }
};
