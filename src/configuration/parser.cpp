#include "era-tue/configuration/parser.h"

#include <fstream>
#include <iostream>

#include <sstream>

namespace configuration
{

// ----------------------------------------------------------------------------------------------------

Parser::Parser(Data& data) : writer_(data), state_(READ_KEY)
{
}

// ----------------------------------------------------------------------------------------------------

Parser::~Parser()
{
}

// ----------------------------------------------------------------------------------------------------

bool Parser::readFile(const std::string& filename)
{
    std::ifstream in;
    in.open(filename.c_str());

    if (!in.is_open())
        return false;

    readStream(in);

    std::string w;
    in >> w;

    in.close();
}

// ----------------------------------------------------------------------------------------------------

bool Parser::readStream(std::istream& in)
{
    char c;
    while(in.read(&c, 1))
    {
        if (c == '#')
            state_ = READ_COMMENT;
        else if (c == '\n')
            state_ = READ_KEY;

        if (state_ == READ_COMMENT)
            continue;

        bool token_end = false;

        switch(c)
        {
        case '\n':
            token_end = true;
            break;
        case ':':
            token_end = true;
            break;
        case ' ':
            token_end = true;
            break;
        case '\t':
            token_end = true;
            break;
        default:
            token_ += c;
        }

        if (token_end && ! token_.empty())
        {
            std::cout << token_ << std::endl;
            token_.clear();
        }

//        std::cout << c << ": " << comment_ << std::endl;
    }
}

// ----------------------------------------------------------------------------------------------------

bool Parser::ok() const
{
    return true;
}


} // end namespace configuration

