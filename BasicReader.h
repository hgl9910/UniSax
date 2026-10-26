#pragma once
#include <fstream>   // std::ifstream
#include <filesystem>
#include <istream>


template<class Info>
class BasicSaxParser;

template<class Info>
class BasicReader
{
public:
    friend class BasicSaxParser<Info>;

    virtual ~BasicReader() = default;

    // filename
    bool Read(std::filesystem::path path)
    {
        std::ifstream input(path);
        if (!input)
            return false;

        return Read(input);
    }
    
	// istream
    bool Read(std::istream& input)
    {
		return ReadStream(input);
    }

    bool ReadStream( std::istream& input) {
		
		typename Info::Parser parser(this);
        return parser.Parse(input);

	}

protected:
    virtual bool OnEvent(const Info& info) = 0;
  //  virtual bool ReadStream(std::istream& input) = 0;
};
