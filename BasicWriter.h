template<class Info>
class BasicWriter
{
public:
    virtual ~BasicWriter() = default;

    bool Write(const std::string& filename);
    bool Write(std::ostream& output);

protected:
    virtual bool WriteStream(std::ostream& output) = 0;
};