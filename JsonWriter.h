```cpp
#pragma once

#include "BasicWriter.h"
#include "JsonInfo.h"

#include <memory>
#include <ostream>
#include <string>
#include <string_view>

class JsonFormatter;

class JsonWriter : public BasicWriter<JsonInfo>
{
public:
    JsonWriter() = default;
    ~JsonWriter() = default;

    bool BeginDocument();
    bool EndDocument();

    bool BeginObject(const std::string& name = {});
    bool EndObject();

    bool BeginArray(const std::string& name = {});
    bool EndArray();

    bool WriteField(const std::string& name, const std::string& value);
    bool WriteField(const std::string& name, std::string_view value);
    bool WriteField(const std::string& name, const char* value);
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
    bool WriteBool(bool value);
    bool WriteNull();

protected:
    bool WriteStream(std::ostream& output) override;
    bool OnEvent(const JsonInfo& info) override;

private:
    bool WriteNumber(const std::string& name,
                     const std::string& value);

    std::ostream* m_output = nullptr;
    std::unique_ptr<JsonFormatter> m_formatter;
};
```
