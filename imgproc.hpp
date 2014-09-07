#ifndef __IMGPROC_HPP
#define __IMGPROC_HPP

#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <iostream>

using namespace cv;
using namespace std;

namespace mhacks
{
    const int threshold_min = 35;
    const int percentageWidthBar = 10;
    enum Direction { Left, Right, Unknown };
    void prepareImage(Mat& m);
    Vec2i processImage(Mat previous, Mat current, Mat next, Mat& processed);
    void imgproc_loop(VideoCapture& cap, function<void (Direction)> processDirection, bool draw=false);
}

#endif
