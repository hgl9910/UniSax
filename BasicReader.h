template<class Info>
class BasicReader
{
public:
    virtual ~BasicReader() = default;

protected:
    virtual bool OnEvent(const Info& info) = 0;
};