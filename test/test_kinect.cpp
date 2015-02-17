#include <iostream>
#include <era/engine.h>
#include <era-tue/kinect.h>

#include <opencv2/highgui/highgui.hpp>

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
