#ifndef ERA_TUE_CONFIGURATION_EMITTER_H_
#define ERA_TUE_CONFIGURATION_EMITTER_H_

#include "era-tue/configuration/data.h"

namespace configuration
{

enum EmitType
{
    YAML,
    JSON
};

void emit(const Data& data, std::ostream& out, EmitType type = YAML);

} // end namespace configuration

#endif
