
class JsonFormatter;
class JsonInfo
{

public:
    using Parser    = JsonParser;
    using Formatter = JsonFormatter;

    enum class Type
    {
        ObjectBegin,
        ObjectEnd,
        ArrayBegin,
        ArrayEnd,
        String,
        Number,
        Boolean,
        Null
    };

    Type type = Type::Null;
    std::string name;
    std::string value;
};


