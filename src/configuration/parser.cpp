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
    std::string key;

    State state = READ_KEY;
    int num_start_spaces = 0;
    int current_num_spaces = 0;
    int line = 1;

    int level = 0;
    std::vector<int> levels;
    levels.push_back(level);

    char c;
    while(in.read(&c, 1))
    {
        if (c != '"' && read_quoted)
        {
            if (c == '\n')
                ++line;

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
            ++line;
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
            --level;
            std::cout << "end group (" << level << ")" << std::endl;
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
            if (state == READ_KEY)
            {
                if (!key.empty())
                {
                    ++level;
                    std::cout << "New group: " << key << " (" << level << ")" << std::endl;

                    if (num_start_spaces >= 0 && num_start_spaces > current_num_spaces)
                    {
                        levels.resize(num_start_spaces + 1);
                        for(unsigned int i = current_num_spaces + 1; i < num_start_spaces; ++i)
                            levels[i] = -1;
                        levels[num_start_spaces] = level;
                    }
                }

                if (num_start_spaces >= 0)
                {
//                    std::cout << num_start_spaces << current_num_spaces << std::endl;



                    if (num_start_spaces < current_num_spaces)
                    {
                        int new_level = levels[num_start_spaces];
                        if (new_level == -1)
                        {
                            std::cout << "Invalid indent level at line " << line << std::endl;
                        }
                        else
                        {
                            for(; level > new_level; --level)
                                std::cout << "end group (" << level - 1 << ")" << std::endl;
                        }
                    }


    //                if (num_start_spaces > current_num_spaces)
    //                    std::cout << "New group" << std::endl;

                    current_num_spaces = num_start_spaces;
                }


                key = token;
            }
            else if (state == READ_VALUE)
            {
                std::cout << "setValue(" << key << ", " << token << ")" << std::endl;
                key.clear();
            }

            if (state == READ_KEY)
                new_state = READ_VALUE;

            token.clear();
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

