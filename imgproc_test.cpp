#include <opencv2/opencv.hpp>
#include <iostream>
#include "imgproc.hpp"

using namespace std;
using namespace cv;

void processDir(mhacks::Direction d)
{
    if (d == mhacks::Right)
        cout << "moving to the right" << endl;
    else
        cout << "moving to the left" << endl;
}

int main(int argc, char **argv)
{
    int capDevice = 0;
    if (argc > 1)
    {
        capDevice = atoi(argv[1]);
    }
    VideoCapture capture(capDevice);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'Y', 'V'));

    if (!capture.isOpened())
    {
        cerr << "Unable to acquire capture source" << endl;
        return 1;
    }

    mhacks::imgproc_loop(capture, processDir, true);

    return 0;
}
