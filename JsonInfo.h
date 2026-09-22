
class JsonParser;

struct json_info
{
    using Parser = JsonParser;

};
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


