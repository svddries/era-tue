#ifndef ERA_TUE_CONFIGURATION_PARSER_H_
#define ERA_TUE_CONFIGURATION_PARSER_H_

#include <string>
#include "era-tue/configuration/writer.h"

namespace configuration
{

class Parser
{

public:

    Parser(Data& data);

    ~Parser();

    bool readFile(const std::string& filename);

    bool readStream(std::istream& in);

    bool ok() const;

private:

    enum State
    {
        READ_KEY,
        READ_VALUE,
        READ_COMMENT
    };

    Writer writer_;

    std::string token_;

    State state_;

};

} // end namespace configuration

#endif
