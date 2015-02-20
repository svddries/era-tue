#include "localizer.h"

#include "era-tue/geometry/primitives.h"

#include <opencv2/highgui/highgui.hpp>

// ----------------------------------------------------------------------------------------------------

Localizer::Localizer()
{
}

// ----------------------------------------------------------------------------------------------------

Localizer::~Localizer()
{
}

// ----------------------------------------------------------------------------------------------------

void Localizer::configure(configuration::Reader& r)
{
    if (r.readGroup("sensor"))
    {
        int width, height;
        r.value("width", width);
        r.value("height", height);

        double fx, fy;
        r.value("fx", fx);
        r.value("fy", fy);

        scale_ = 10;

        rasterizer_.setCanvasSize(width / scale_, height / scale_);
        rasterizer_.setFocalLengths(fx / scale_, fy / scale_);
        rasterizer_.setOpticalCenter(width / 2 / scale_, height / 2 / scale_);
        rasterizer_.setOpticalTranslation(0, 0);

        r.endGroup();
    }

    geometry::Mesh box;
    geometry::createBox(geometry::Vec3f(-0.25, -0.15, -0.1), geometry::Vec3f(0.25, 0.15, 0.1), box);
    world_meshes_.push_back(box);

    pose_ = geometry::Transform3f(geometry::Mat3f::identity(), geometry::Vec3f(0, 0, -1.0));
}

// ----------------------------------------------------------------------------------------------------

void Localizer::check(const RGBDImage& image)
{
    int best_score = 0;
    cv::Mat best_render;
    geometry::Transform3f best_pose;

    int n = 0;
    for(double dx = -0.1; dx < 0.1; dx += 0.05)
    {
    for(double dy = -0.1; dy < 0.1; dy += 0.05)
    {

    for(double dz = -0.1; dz < 0.1; dz += 0.05)
    {
        for(double a = -0.2; a < 0.2; a += 0.05)
        {
            geometry::Mat3f m;
            m.setRPY(0, a, 0);

            geometry::Transform3f new_pose = pose_;
            new_pose.t.x += dx;
            new_pose.t.y += dy;
            new_pose.t.z += dz;
            new_pose.R = new_pose.R * m;

            cv::Mat render(480 / scale_, 640 / scale_, CV_32FC1, 0.0);

            for(std::vector<geometry::Mesh>::const_iterator it = world_meshes_.begin(); it != world_meshes_.end(); ++it)
            {
                rasterizer_.rasterize(*it, new_pose, (float*)render.data);
            }

            int score = 0;
            for(int y = 0; y < 480 / scale_; ++y)
            {
                for(int x = 0; x < 640 / scale_; ++x)
                {
                    float ds = image.depth.at<float>(scale_ * y, scale_ * x);
                    float dm = render.at<float>(y, x);

                    if (dm > 0 && std::abs(ds - dm) < 0.01)
                        ++score;
                }
            }

            if (score > best_score)
            {
                best_score = score;
                best_render = render;
                best_pose = new_pose;
            }

            ++n;
        }
    }
    }
    }

    pose_ = best_pose;

    std::cout << n << " renders, best score = " << best_score << std::endl;

    if (best_render.data)
    {
        cv::imshow("best render", best_render / 2);
//        cv::waitKey(3);
    }
}

// ----------------------------------------------------------------------------------------------------

