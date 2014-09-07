#include "imgproc.hpp"

void mhacks::prepareImage(Mat& m)
{
    //normalize(m, m, 0, 255, NORM_MINMAX, CV_8UC1);
    vector<Mat> channels;
    split(m, channels);
    m = channels[0]; // should be Y channel
    blur(m, m, Size(3, 3));
}

Vec2i mhacks::processImage(Mat previous, Mat current, Mat next, Mat& processed)
{
    Mat d1, d2, changeMap;
    absdiff(previous, current, d1);
    absdiff(current, next, d2);
    bitwise_and(d1, d2, changeMap);
    medianBlur(changeMap, changeMap, 5);
    threshold(changeMap, changeMap, threshold_min, 255, CV_THRESH_BINARY);
    erode(changeMap, changeMap, getStructuringElement(MORPH_RECT, Size(2, 2)));

    Moments imageMoment = moments(changeMap);
    int xAvg = imageMoment.m10/imageMoment.m00;
    int yAvg = imageMoment.m01/imageMoment.m00;
    
    processed = changeMap;

    return Vec2i(xAvg, yAvg);
}

void mhacks::imgproc_loop(VideoCapture& cap, function<void (mhacks::Direction)> processDirection, bool draw)
{
    if (draw)
    {
        namedWindow("Processed Frame", CV_WINDOW_AUTOSIZE);
    }
    Mat previous;
    Mat current;
    Mat next;
    Mat processed;
    long startTime, endTime, frameCounter = 0;
    float sec, fps;

    Vec2i firstSample;
    Vec2i lastSample;
    bool needFirstSample = true;
    bool justExitedRight = false;
    bool justExitedLeft = false;
    Direction dir = Unknown;
    cap >> previous >> current >> next;
    prepareImage(previous);
    prepareImage(current);
    prepareImage(next);
    processImage(previous, current, next, processed);
    int leftThresh = current.cols/10;
    int rightThresh = current.cols - leftThresh;
    startTime = getTickCount();
    for (;;)
    {
        previous = current;
        current = next;
        cap >> next;
        prepareImage(next);
        Vec2i pos = processImage(previous, current, next, processed);
        if (needFirstSample && pos[0] >= 0 && pos[1] >= 0)
        {
            if (!justExitedLeft && !justExitedRight)
            {
                firstSample = pos;
                justExitedLeft = false;
                justExitedRight = false;
                needFirstSample = false;
            }
            if (pos[0] <= leftThresh && !justExitedLeft)
            {
                firstSample = pos;
                justExitedLeft = false;
                justExitedRight = false;
                needFirstSample = false;
            }
            if (pos[0] >= rightThresh && !justExitedRight)
            {
                firstSample = pos;
                justExitedLeft = false;
                justExitedRight = false;
                needFirstSample = false;
            }
        }
        else if (!needFirstSample)
        {
            if (pos[0] > 0 && pos[1] > 0)
            {
                if (pos[0] <= leftThresh && pos[0] < firstSample[0])
                {
                    lastSample = pos;
                    dir = Left;
                    justExitedLeft = true;
                }
                else if (pos[0] >= rightThresh && pos[0] > firstSample[0])
                {
                    lastSample = pos;
                    dir = Right;
                    justExitedRight = true;
                }
            }
        }

        if (dir != Unknown)
        {
            processDirection(dir);
            needFirstSample = true;
            firstSample = Vec2i(-1, -1);
            lastSample = Vec2i(-1, -1);
            dir = Unknown;
        }

        endTime = getTickCount();
        ++frameCounter;
        if (frameCounter % 30 == 0)
        {
            sec = ((float)endTime-startTime)/((float)getTickFrequency());
            fps = (float)frameCounter/sec;
            cout << "FPS: " << fps << endl;
            frameCounter = 0;
            startTime = getTickCount();
        }

        if (draw)
        {
            if (pos[0] >= 0 && pos[1] >= 0)
                circle(current, pos, 8, Scalar(255), 2, 8, 0);
            imshow("Processed Frame", current);
            if (waitKey(10) >= 0) break;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }
}
