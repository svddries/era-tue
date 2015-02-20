#include "era-tue/configuration/emitter.h"
#include <iostream>

namespace configuration
{

// ----------------------------------------------------------------------------------------------------

Emitter::Emitter(Data& data) : data_(data)
{
}

// ----------------------------------------------------------------------------------------------------

Emitter::~Emitter()
{
}

// ----------------------------------------------------------------------------------------------------

void emitMap(const Data& data, const std::map<std::string, Node>& map, const std::string& indent, bool put_first_indent, std::ostream& out)
{
    for(std::map<std::string, Node>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        if (put_first_indent)
            out << indent;

        out << it->first << ": ";

        const Node& n = it->second;
        if (n.type == VALUE)
            out << data.values[n.idx] << std::endl;
        else if (n.type == MAP)
        {
            out << std::endl;
            emitMap(data, data.maps[n.idx], indent + "    ", true, out);
        }
        else if (n.type == ARRAY)
        {
            out << std::endl;
            const std::vector<Node>& array = data.arrays[n.idx];
            for(std::vector<Node>::const_iterator it = array.begin(); it != array.end(); ++it)
            {
                out << indent << "  - ";
                emitMap(data, data.maps[it->idx], indent + "    ", false, out);
            }
        }

        put_first_indent = true;
    }
}

// ----------------------------------------------------------------------------------------------------

void Emitter::emit(std::ostream& out)
{
    if (data_.maps.empty())
        return;

    std::string indent = "";
    emitMap(data_, data_.maps[0], indent, true, out);
}

} // end namespace configuration

