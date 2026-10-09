#pragma once

#include "BasicFormatter.h"
#include "JsonInfo.h"

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

class JsonFormatter : public BasicFormatter<JsonInfo>
{
public:
    explicit JsonFormatter(std::ostream& output);

    bool Format(const JsonInfo& info) override;

private:
    enum class ContainerType
    {
        Object,
        Array
    };

    struct Context
    {
        ContainerType type;
        bool first = true;
    };

    bool BeginObject(const std::string& name);
    bool EndObject();

    bool BeginArray(const std::string& name);
    bool EndArray();

    bool WriteField(const std::string& name,
                    const std::string& value);

    bool WriteField(const std::string& name,
                    std::string_view value);

    bool WriteField(const std::string& name,
                    const char* value);

    bool WriteField(const std::string& name, int value);
    bool WriteField(const std::string& name, unsigned value);
    bool WriteField(const std::string& name, long long value);

    bool WriteField(const std::string& name,
                    unsigned long long value);

    bool WriteField(const std::string& name, double value);
    bool WriteField(const std::string& name, bool value);

    bool WriteNull(const std::string& name);

    bool WriteString(const std::string& value);

    bool WriteNumber(const std::string& value);

    bool WriteNumber(const std::string& name,
                     const std::string& value);

    bool WriteBool(bool value);
    bool WriteNull();

    void BeforeValue(const std::string& name = {});
    void Indent();

    static std::string Escape(const std::string& value);

    std::ostream& m_output;
    std::vector<Context> m_stack;
};