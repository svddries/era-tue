#include "era-tue/kinect.h"

RGBDImage Kinect::image_;
bool Kinect::new_image_;

// ----------------------------------------------------------------------------------------------------

Kinect::Kinect()
{
}

// ----------------------------------------------------------------------------------------------------

Kinect::~Kinect()
{
}

// ----------------------------------------------------------------------------------------------------

void Kinect::depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
    image_.depth = cv::Mat(480, 640, CV_32FC1, 0.0);

    uint16_t *depth = (uint16_t*)v_depth;

    int i = 0;
    for(int y = 0; y < 480; ++y)
    {
        for(int x = 0; x < 640; ++x)
        {
            if (depth[i] > 0)
            {
                float d = (float)depth[i] / 1000;
                //            std::cout << d << std::endl;
                image_.depth.at<float>(y, x) = d;
            }
            ++i;
        }
    }

    new_image_ = true;
}

// ----------------------------------------------------------------------------------------------------

void Kinect::rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    uint8_t *rgb_mid = (uint8_t*)rgb;

    image_.rgb = cv::Mat(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));

    int i = 0;
    for(int y = 0; y < 480; ++y)
    {
        for(int x = 0; x < 640; ++x)
        {
            image_.rgb.at<cv::Vec3b>(y, x) = cv::Vec3b(rgb_mid[i+2], rgb_mid[i+1], rgb_mid[i]);
            i += 3;
        }
    }

    new_image_ = true;
}

// ----------------------------------------------------------------------------------------------------

void Kinect::initialize(era::InitializationData& init)
{
    p_kinect_ = init.blackboard->openBuffer<RGBDImage>("kinect");

    // Initialize freenect
    if (freenect_init(&f_ctx, NULL) < 0)
    {
        std::cout << "freenect_init() failed" << std::endl;
        return;
    }

    freenect_set_log_level(f_ctx, FREENECT_LOG_ERROR);
    freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_CAMERA));

    int nr_devices = freenect_num_devices (f_ctx);
    std::cout << "Number of devices found: " << nr_devices << std::endl;

    if (nr_devices < 1) {
        freenect_shutdown(f_ctx);
        return;
    }

    int user_device_number = 0;
    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0)
    {
        std::cout << "Could not open device" << std::endl;
        freenect_shutdown(f_ctx);
        return;
    }

    Kinect& f = *this;

    freenect_set_depth_callback(f_dev, this->depth_cb);
    freenect_set_video_callback(f_dev, this->rgb_cb);
    freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED));

    freenect_start_depth(f_dev);
    freenect_start_video(f_dev);

    time_ = 0;
}


// ----------------------------------------------------------------------------------------------------

void Kinect::process()
{
    new_image_ = false;

    int res = freenect_process_events(f_ctx);
    if (res < 0 && res != -10)
    {
        std::cout << "\nError " << res << " received from libusb - aborting." << std::endl;
        return;
    }

    if (new_image_)
    {
        p_kinect_.buffer->insert(time_, image_);
        time_ += 1;
    }
}

// ----------------------------------------------------------------------------------------------------

void Kinect::stop()
{
    std::cout << "Kinect stopped" << std::endl;

    freenect_stop_depth(f_dev);
    freenect_stop_video(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);
}

