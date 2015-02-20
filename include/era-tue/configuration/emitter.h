#ifndef ERA_TUE_CONFIGURATION_EMITTER_H_
#define ERA_TUE_CONFIGURATION_EMITTER_H_

#include "era-tue/configuration/data.h"

namespace configuration
{

class Emitter
{

public:

    Emitter(Data& data);

    ~Emitter();

    void emit(std::ostream& out);

private:

    Data& data_;

};

} // end namespace configuration

#endif
