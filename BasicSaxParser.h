#include BasicReader.h

template<class Info>
class BasicSaxParser
{
public:
    explicit BasicSaxParser(BasicReader<Info>* reader)
        : m_reader(reader)
    {
    }

protected:
    BasicReader<Info>* m_reader = nullptr;

    bool OnEvent(const Info& info)
    {
        return m_reader ? m_reader->OnEvent(info) : false;
    }
};

