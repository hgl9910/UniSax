
class CJsonParser
{
public:
    virtual ~CJsonReader() = default;

   bool Parse();

protected:
    virtual bool ReadField(const json_info& info) = 0;

private:
    std::ifstream m_file;
    std::string m_data;
    std::size_t m_pos = 0;

    bool ParseValue(const std::string& name = {});
    bool ParseObject();
    bool ParseArray();
    bool ParseString(std::string& result);
    bool ParseNumber(std::string& result);
    bool ParseLiteral(std::string& result);

    void SkipWhitespace();
    bool End() const;
    char Current() const;
    bool Consume(char c);

    bool Emit(json_info::Type type,
              const std::string& name = {},
              const std::string& value = {});

    static bool IsNumberChar(char c);
    static bool IsHexDigit(char c);
    static unsigned HexValue(char c);
    static void AppendUtf8(std::string& out, unsigned codepoint);
};