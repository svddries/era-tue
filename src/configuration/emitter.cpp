#include "era-tue/configuration/emitter.h"
#include <iostream>

namespace configuration
{

// ----------------------------------------------------------------------------------------------------

void emitYAML(const Data& data, const std::map<std::string, Node>& map, const std::string& indent, bool put_first_indent, std::ostream& out)
{
    for(std::map<std::string, Node>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        if (put_first_indent)
            out << indent;

        out << it->first << ":";

        const Node& n = it->second;
        if (n.type == VALUE)
            out << " " << data.values[n.idx] << std::endl;
        else if (n.type == MAP)
        {
            out << std::endl;
            emitYAML(data, data.maps[n.idx], indent + "    ", true, out);
        }
        else if (n.type == ARRAY)
        {
            out << std::endl;
            const std::vector<Node>& array = data.arrays[n.idx];
            for(std::vector<Node>::const_iterator it = array.begin(); it != array.end(); ++it)
            {
                out << indent << "  - ";
                emitYAML(data, data.maps[it->idx], indent + "    ", false, out);
            }
        }

        put_first_indent = true;
    }
}

// ----------------------------------------------------------------------------------------------------

void emitJSON(const Data& data, const std::map<std::string, Node>& map, const std::string& indent, std::ostream& out)
{
    out << "{" << std::endl;

    std::string new_indent = indent + "    ";

    for(std::map<std::string, Node>::const_iterator it = map.begin(); it != map.end(); ++it)
    {
        if (it != map.begin())
            out << "," << std::endl;

        out << new_indent << "\"" << it->first << "\": ";

        const Node& n = it->second;
        if (n.type == VALUE)
            out << data.values[n.idx];// << std::endl;
        else if (n.type == MAP)
        {
            emitJSON(data, data.maps[n.idx], new_indent, out);
        }
        else if (n.type == ARRAY)
        {
            out << "[" << std::endl;
            const std::vector<Node>& array = data.arrays[n.idx];
            for(std::vector<Node>::const_iterator it2 = array.begin(); it2 != array.end(); ++it2)
            {
                if (it2 != array.begin())
                    out << "," << std::endl;

                out << new_indent + "    ";
                emitJSON(data, data.maps[it2->idx], new_indent + "    ", out);
            }
            out << std::endl << new_indent << "]";
        }
    }

    out << std::endl << indent << "}";
}

// ----------------------------------------------------------------------------------------------------

void emit(const Data& data, std::ostream& out, EmitType type)
{
    if (data.maps.empty())
        return;

    std::string indent = "";

    if (type == YAML)
        emitYAML(data, data.maps[0], indent, true, out);
    else if (type == JSON)
        emitJSON(data, data.maps[0], indent, out);
}

} // end namespace configuration

