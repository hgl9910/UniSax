#include "JsonWriter.h"
#include "JsonFormatter.h"

#include <iomanip>
#include <sstream>

bool JsonWriter::WriteStream(std::ostream& output)
{
    if (!output)
        return false;

    m_output = &output;

    return true;
}

bool JsonWriter::BeginDocument()
{
    if (!m_output || m_formatter)
        return false;

    m_formatter =
        std::make_unique<JsonFormatter>(*m_output);

    JsonInfo info;
    info.type = JsonInfo::Type::DocumentBegin;

    return OnEvent(info);
}

bool JsonWriter::EndDocument()
{
    if (!m_formatter)
        return false;

    JsonInfo info;
    info.type = JsonInfo::Type::DocumentEnd;

    const bool result = OnEvent(info);

    m_formatter.reset();

    return result;
}

bool JsonWriter::OnEvent(const JsonInfo& info)
{
    if (!m_formatter)
        return false;

    return m_formatter->Format(info);
}

bool JsonWriter::BeginObject(const std::string& name)
{
    JsonInfo info;

    info.type = JsonInfo::Type::ObjectBegin;
    info.name = name;

    return OnEvent(info);
}

bool JsonWriter::EndObject()
{
    JsonInfo info;

    info.type = JsonInfo::Type::ObjectEnd;

    return OnEvent(info);
}

bool JsonWriter::BeginArray(const std::string& name)
{
    JsonInfo info;

    info.type = JsonInfo::Type::ArrayBegin;
    info.name = name;

    return OnEvent(info);
}

bool JsonWriter::EndArray()
{
    JsonInfo info;

    info.type = JsonInfo::Type::ArrayEnd;

    return OnEvent(info);
}

bool JsonWriter::WriteField(const std::string& name,
                            const std::string& value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::String;
    info.name = name;
    info.value = value;

    return OnEvent(info);
}

bool JsonWriter::WriteField(const std::string& name,
                            std::string_view value)
{
    return WriteField(name, std::string(value));
}

bool JsonWriter::WriteField(const std::string& name,
                            const char* value)
{
    return WriteField(
        name,
        std::string(value ? value : ""));
}

bool JsonWriter::WriteField(const std::string& name,
                            int value)
{
    return WriteNumber(
        name,
        std::to_string(value));
}

bool JsonWriter::WriteField(const std::string& name,
                            unsigned value)
{
    return WriteNumber(
        name,
        std::to_string(value));
}

bool JsonWriter::WriteField(const std::string& name,
                            long long value)
{
    return WriteNumber(
        name,
        std::to_string(value));
}

bool JsonWriter::WriteField(
    const std::string& name,
    unsigned long long value)
{
    return WriteNumber(
        name,
        std::to_string(value));
}

bool JsonWriter::WriteField(const std::string& name,
                            double value)
{
    std::ostringstream out;

    out << std::setprecision(17)
        << value;

    return WriteNumber(name, out.str());
}

bool JsonWriter::WriteField(const std::string& name,
                            bool value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::Boolean;
    info.name = name;
    info.value = value ? "true" : "false";

    return OnEvent(info);
}

bool JsonWriter::WriteNull(const std::string& name)
{
    JsonInfo info;

    info.type = JsonInfo::Type::Null;
    info.name = name;

    return OnEvent(info);
}

bool JsonWriter::WriteString(const std::string& value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::String;
    info.value = value;

    return OnEvent(info);
}

bool JsonWriter::WriteNumber(const std::string& value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::Number;
    info.value = value;

    return OnEvent(info);
}

bool JsonWriter::WriteNumber(const std::string& name,
                             const std::string& value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::Number;
    info.name = name;
    info.value = value;

    return OnEvent(info);
}

bool JsonWriter::WriteBool(bool value)
{
    JsonInfo info;

    info.type = JsonInfo::Type::Boolean;
    info.value = value ? "true" : "false";

    return OnEvent(info);
}

bool JsonWriter::WriteNull()
{
    JsonInfo info;

    info.type = JsonInfo::Type::Null;

    return OnEvent(info);
}