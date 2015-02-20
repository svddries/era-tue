#include <iostream>

#include <opencv2/highgui/highgui.hpp>

#include "era-tue/geometry/mesh.h"
#include "era-tue/geometry/rasterizer.h"
#include "era-tue/geometry/primitives.h"

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    geometry::Mesh mesh;
    geometry::createBox(geometry::Vec3f(-0.2, -0.2, -0.2), geometry::Vec3f(0.2, 0.2, 0.2), mesh);

    geometry::Rasterizer rasterizer;
    rasterizer.setCanvasSize(640, 480);
    rasterizer.setFocalLengths(558, 558);
    rasterizer.setOpticalCenter(320, 240);
    rasterizer.setOpticalTranslation(0, 0);

    for(double a = 0; true; a += 0.02)
    {
        geometry::Mat3f m;
        m.setRPY(0, a, a / 2);

        geometry::Transform3f pose(m, geometry::Vec3f(0, 0, -1));

        cv::Mat depth_image(480, 640, CV_32FC1, 0.0);

        rasterizer.rasterize(mesh, pose, (float*)depth_image.data);

        cv::imshow("render", depth_image / 2);
        cv::waitKey(3);
    }

    return 0;
}

