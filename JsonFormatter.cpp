
bool JsonFormatter::Format(const JsonInfo& info)
{
    switch (info.type)
    {
    case JsonInfo::Type::DocumentBegin:
        return true;

    case JsonInfo::Type::DocumentEnd:
        return static_cast<bool>(m_output);

    case JsonInfo::Type::ObjectBegin:
        return BeginObject(info.name);

    case JsonInfo::Type::ObjectEnd:
        return EndObject();

    case JsonInfo::Type::ArrayBegin:
        return BeginArray(info.name);

    case JsonInfo::Type::ArrayEnd:
        return EndArray();

    case JsonInfo::Type::String:
        return WriteField(info.name, info.value);

    case JsonInfo::Type::Number:
        return WriteNumber(info.name, info.value);

    case JsonInfo::Type::Boolean:
        return WriteField(info.name, info.value == "true");

    case JsonInfo::Type::Null:
        return WriteNull(info.name);
    }

    return false;
}

ool JsonFormatter::BeginObject(const std::string& name)
{
    BeforeValue(name);
    m_file << "{\n";
    m_stack.push_back({ ContainerType::Object, true });
    return static_cast<bool>(m_file);
}

bool JsonFormatter::EndObject()
{
    if (m_stack.empty() ||
        m_stack.back().type != ContainerType::Object)
        return false;

    const bool first = m_stack.back().first;
    m_stack.pop_back();

    if (!first)
    {
        m_file << '\n';
        Indent();
    }

    m_file << '}';
    return static_cast<bool>(m_file);
}

bool JsonFormatter::BeginArray(const std::string& name)
{
    BeforeValue(name);
    m_file << "[\n";
    m_stack.push_back({ ContainerType::Array, true });
    return static_cast<bool>(m_file);
}

bool JsonFormatter::EndArray()
{
    if (m_stack.empty() ||
        m_stack.back().type != ContainerType::Array)
        return false;

    const bool first = m_stack.back().first;
    m_stack.pop_back();

    if (!first)
    {
        m_file << '\n';
        Indent();
    }

    m_file << ']';
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteField(const std::string& name,
                             const std::string& value)
{
    BeforeValue(name);
    m_file << '"' << Escape(value) << '"';
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteField(const std::string& name,
                             std::string_view value)
{
    return WriteField(name, std::string(value));
}

bool JsonFormatter::WriteField(const std::string& name,
                             const char* value)
{
    return WriteField(name, std::string(value ? value : ""));
}

bool JsonFormatter::WriteField(const std::string& name, int value)
{
    return WriteNumber([&]
    {
        return std::to_string(value);
    }(), name);
}

bool JsonFormatter::WriteField(const std::string& name, unsigned value)
{
    return WriteNumber(std::to_string(value), name);
}

bool JsonFormatter::WriteField(const std::string& name, long long value)
{
    return WriteNumber(std::to_string(value), name);
}

bool JsonFormatter::WriteField(const std::string& name,
                             unsigned long long value)
{
    return WriteNumber(std::to_string(value), name);
}

bool JsonFormatter::WriteField(const std::string& name, double value)
{
    std::ostringstream out;
    out << std::setprecision(17) << value;
    return WriteNumber(out.str(), name);
}

bool JsonFormatter::WriteField(const std::string& name, bool value)
{
    BeforeValue(name);
    m_file << (value ? "true" : "false");
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteNull(const std::string& name)
{
    BeforeValue(name);
    m_file << "null";
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteString(const std::string& value)
{
    BeforeValue();
    m_file << '"' << Escape(value) << '"';
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteNumber(const std::string& value)
{
    BeforeValue();
    m_file << value;
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteNumber(const std::string& name,
                              const std::string& value)
{
    BeforeValue(name);
    m_file << value;
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteBool(bool value)
{
    BeforeValue();
    m_file << (value ? "true" : "false");
    return static_cast<bool>(m_file);
}

bool JsonFormatter::WriteNull()
{
    BeforeValue();
    m_file << "null";
    return static_cast<bool>(m_file);
}

void JsonFormatter::BeforeValue(const std::string& name)
{
    if (m_stack.empty())
        return;

    Context& ctx = m_stack.back();

    if (!ctx.first)
        m_file << ",\n";

    Indent();

    if (ctx.type == ContainerType::Object)
    {
        m_file << '"' << Escape(name) << "\": ";
    }

    ctx.first = false;
}

void JsonFormatter::Indent()
{
    const std::size_t level = m_stack.size();

    for (std::size_t i = 0; i < level; ++i)
        m_file << "    ";
}

std::string JsonFormatter::Escape(const std::string& value)
{
    std::string result;
    result.reserve(value.size());

    for (unsigned char c : value)
    {
        switch (c)
        {
        case '"':  result += "\\\""; break;
        case '\\': result += "\\\\"; break;
        case '\b': result += "\\b";  break;
        case '\f': result += "\\f";  break;
        case '\n': result += "\\n";  break;
        case '\r': result += "\\r";  break;
        case '\t': result += "\\t";  break;

        default:
            if (c < 0x20)
            {
                const char* hex = "0123456789abcdef";
                result += "\\u00";
                result += hex[(c >> 4) & 0x0F];
                result += hex[c & 0x0F];
            }
            else
            {
                result += static_cast<char>(c);
            }
            break;
        }
    }

    return result;
}
