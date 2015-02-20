#include "era-tue/configuration/parser.h"

#include <fstream>
#include <iostream>

namespace configuration
{

char readWhitespace(char c, std::istream& in, int& num_spaces)
{
    do
    {
        if (c == ' ')
            num_spaces += 1;
        else if (c == '\t')
            num_spaces += 4;
        else
            return c;
    } while(in.read(&c, 1));

    return ' '; //  TODO
}

// ----------------------------------------------------------------------------------------------------

Parser::Parser(Data& data) : writer_(data)
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
    bool read_quoted = false;

    std::string token;

    State state = READ_KEY;
    int num_start_spaces = 0;

    char c;
    while(in.read(&c, 1))
    {
        if (c != '"' && read_quoted)
        {
            token += c;
            continue;
        }

        if (c == '#')
            state = READ_COMMENT;

        if (state == READ_COMMENT)
            continue;

        State new_state = state;

        if (state == READ_NEWLINE_WHITESPACE)
        {
            num_start_spaces = 0;
            c = readWhitespace(c, in, num_start_spaces);
            new_state = READ_KEY;
        }

        bool token_end = true;

        switch(c)
        {
        case '"':
            read_quoted = !read_quoted;
            break;
        case '\n':
            new_state = READ_NEWLINE_WHITESPACE;
        case ':':
            break;
        case '=':
            break;
        case ' ':
            break;
        case '\t':
            break;
        case '{':
            new_state = READ_KEY;
            break;
        case '}':
            break;
        case ',':
            new_state = READ_KEY;
            break;
        default:
            token += c;
            token_end = false;
        }

        if (token_end && ! token.empty())
        {
            std::cout << token << " ";
            token.clear();

            if (state == READ_KEY)
                std::cout << "(key, space = " << num_start_spaces << ")";
            else if (state == READ_VALUE)
                std::cout << "(value)";
            std::cout << std::endl;

            if (state == READ_KEY)
                new_state = READ_VALUE;

            num_start_spaces = -1;
        }

        state = new_state;
    }
}

// ----------------------------------------------------------------------------------------------------

bool Parser::ok() const
{
    return true;
}


} // end namespace configuration

