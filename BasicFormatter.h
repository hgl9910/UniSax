template<class Info>
class BasicFormatter
{
public:
    virtual ~BasicFormatter() = default;

    virtual bool Format(const Info& info) = 0;
};