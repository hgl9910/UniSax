template<class Info>
class BasicWriter
{
public:
    virtual ~BasicWriter() = default;

   
	    // filename
    bool Write(const std::string& filename)
    {
        std::ofstream output(filename);
        if (!output)
            return false;

        return WriteStream(output);
    }
 
	
	
    bool Write(std::ostream& output)
	{
		return WriteStream(output);	
	}

protected:
    virtual bool WriteStream(std::ostream& output) = 0;
};

