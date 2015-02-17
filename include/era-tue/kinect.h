#ifndef ERA_TUE_KINECT_H_
#define ERA_TUE_KINECT_H_

#include <era/component.h>
#include <opencv2/core/core.hpp>

#include <libfreenect.h>

// ----------------------------------------------------------------------------------------------------

struct RGBDImage
{
    cv::Mat rgb;
    cv::Mat depth;
};

// ----------------------------------------------------------------------------------------------------

class Kinect : public era::Component
{

public:

    Kinect();

    ~Kinect();

    void initialize(era::InitializationData& init);

    void process();

    void stop();

private:

    era::Port<RGBDImage> p_kinect_;

    freenect_context *f_ctx;
    freenect_device *f_dev;

    double time_;

    static void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp);

    static void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp);

    static RGBDImage image_;

    static bool new_image_;

};

#endif
