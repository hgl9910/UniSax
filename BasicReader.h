
template<class Info>
class BasicReader
{
public:
    virtual ~BasicReader() = default;

    // filename
    bool Read(const std::string& filename)
    {
        std::ifstream input(filename);
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
