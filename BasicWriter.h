#pragma once

#include <fstream>
#include <ostream>
#include <string>

template<class Info>
class BasicWriter
{
public:
    virtual ~BasicWriter() = default;

    bool Write(const std::string& filename)
    {
        m_file.close();
        m_file.clear();

        m_file.open(filename);

        if (!m_file)
            return false;

        return WriteStream(m_file);
    }

    bool Write(std::ostream& output)
    {
        return WriteStream(output);
    }

protected:
    virtual bool WriteStream(std::ostream& output) = 0;

    virtual bool OnEvent(const Info& info) = 0;

private:
    std::ofstream m_file;
};