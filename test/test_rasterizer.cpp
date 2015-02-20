#include <iostream>
#include <era/engine.h>
#include <era-tue/kinect.h>

// Configuration
#include <era-tue/configuration/parser.h>
#include <era-tue/configuration/reader.h>

#include <opencv2/highgui/highgui.hpp>

#include "era-tue/geometry/mesh.h"
#include "era-tue/geometry/rasterizer.h"
#include "era-tue/geometry/primitives.h"

#include <signal.h>

// ----------------------------------------------------------------------------------------------------

class KinectViewer : public era::Component
{

public:

    void initialize(era::InitializationData& init)
    {
        p_kinect_ = init.blackboard->openBuffer<RGBDImage>("kinect");
        t_last_ = 0;
    }

    void process()
    {
        RGBDImage image;
        era::Time time;
        if (p_kinect_.buffer->latest(image, time) && time > t_last_)
        {
            if (image.rgb.data)
                cv::imshow("rgb", image.rgb);

            if (image.depth.data)
                cv::imshow("depth", image.depth / 8);

            cv::waitKey(3);

            t_last_ = time;
        }
    }

private:

    era::Port<RGBDImage> p_kinect_;
    era::Time t_last_;

};

// ----------------------------------------------------------------------------------------------------

bool finished = false;

void signal_callback_handler(int signum)
{
   finished = true;
}

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

    if (argc < 2)
    {
        std::cout << "Please provide a configuration file." << std::endl;
        return 1;
    }

    configuration::Data config;
    configuration::Parser parser(config);
    if (!parser.readFile(argv[1]))
    {
        std::cout << std::endl << "Could not successfully parse '" << argv[1] << "'" << std::endl;
        return 1;
    }

    // Register signal and signal handler
    signal(SIGINT, signal_callback_handler);

    // Create ERA engine
    era::Engine engine;

    // Register kinect and kinect_viewer components
    engine.registerComponent("kinect", new Kinect);
    engine.registerComponent("kinect_viewer", new KinectViewer);

    // Run until user interrupts
    while(!finished)
    {
        engine.step();
    }

    return 0;
}

