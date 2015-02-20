#ifndef WM_LOCALIZER_H_
#define WM_LOCALIZER_H_

#include "era-tue/geometry/rasterizer.h"

class Localizer
{

public:

    Localizer();

    ~Localizer();

private:

    geometry::Rasterizer rasterizer_;

};

#endif
