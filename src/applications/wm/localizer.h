#ifndef WM_LOCALIZER_H_
#define WM_LOCALIZER_H_

#include "era-tue/configuration/reader.h"

#include "era-tue/geometry/rasterizer.h"
#include "era-tue/geometry/mesh.h"

#include "era-tue/kinect.h" // for RGBDImage struct

class Localizer
{

public:

    Localizer();

    ~Localizer();

    void configure(configuration::Reader& r);

    void check(const RGBDImage& image);

private:

    geometry::Rasterizer rasterizer_;

    std::vector<geometry::Mesh> world_meshes_;

    geometry::Transform3f pose_;

    double scale_;

};

#endif
