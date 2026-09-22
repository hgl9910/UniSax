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
        std::ofstream output(filename);

        if (!output)
            return false;

        return Write(output);
    }

    bool Write(std::ostream& output)
    {
        return WriteStream(output);
    }

protected:
    virtual bool WriteStream(std::ostream& output) = 0;

    virtual bool BeginDocument(std::ostream& output) = 0;
    virtual bool EndDocument() = 0;

    virtual bool OnEvent(const Info& info) = 0;
};