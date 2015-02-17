#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libfreenect.h"

#include <pthread.h>
#include <math.h>

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

pthread_t freenect_thread;
volatile int die = 0;

uint8_t *rgb_back;

freenect_context *f_ctx;
freenect_device *f_dev;

int rgb_seq = 0;
int depth_seq = 0;

// ----------------------------------------------------------------------------------------------------

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
    cv::Mat depth_image(480, 640, CV_32FC1, 0.0);

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
                depth_image.at<float>(y, x) = d;
            }
            ++i;
        }
    }

    depth_seq++;

    cv::imshow("depth", depth_image / 8);
    cv::waitKey(3);
}

// ----------------------------------------------------------------------------------------------------

void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
    uint8_t *rgb_mid = (uint8_t*)rgb;

    cv::Mat rgb_image(480, 640, CV_8UC3, cv::Scalar(0, 0, 0));

    int i = 0;
    for(int y = 0; y < 480; ++y)
    {
        for(int x = 0; x < 640; ++x)
        {
            rgb_image.at<cv::Vec3b>(y, x) = cv::Vec3b(rgb_mid[i+2], rgb_mid[i+1], rgb_mid[i]);
            i += 3;
        }
    }

    cv::imshow("rgb", rgb_image);
    cv::waitKey(3);

    rgb_seq++;
}

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{

    printf("Kinect camera test\n");


    if (freenect_init(&f_ctx, NULL) < 0) {
        printf("freenect_init() failed\n");
        return 1;
    }

    freenect_set_log_level(f_ctx, FREENECT_LOG_ERROR);
    freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_CAMERA));

    int nr_devices = freenect_num_devices (f_ctx);
    printf ("Number of devices found: %d\n", nr_devices);

    int user_device_number = 0;
    if (argc > 1)
        user_device_number = atoi(argv[1]);

    if (nr_devices < 1) {
        freenect_shutdown(f_ctx);
        return 1;
    }

    if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
        printf("Could not open device\n");
        freenect_shutdown(f_ctx);
        return 1;
    }

    // -----

    freenect_set_depth_callback(f_dev, depth_cb);
    freenect_set_video_callback(f_dev, rgb_cb);
    freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB));
    freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED));
    freenect_set_video_buffer(f_dev, rgb_back);

    freenect_start_depth(f_dev);
    freenect_start_video(f_dev);

    while (!die)
    {
        int res = freenect_process_events(f_ctx);
        if (res < 0 && res != -10)
        {
            printf("\nError %d received from libusb - aborting.\n",res);
            break;
        }
    }
    printf("\nshutting down streams...\n");

    freenect_stop_depth(f_dev);
    freenect_stop_video(f_dev);

    freenect_close_device(f_dev);
    freenect_shutdown(f_ctx);

    printf("-- done!\n");

    // -----

    return 0;
}
