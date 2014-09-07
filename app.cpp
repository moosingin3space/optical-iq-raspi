#include "api.hpp"
#include "settings.hpp"
#include "imgproc.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>

using namespace std;
using namespace cv;

mhacks::settings globalSettings;

void submitThread(atomic<int>& delta)
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        // now send deltas
        if (delta != 0)
            mhacks::send_delta((int)delta, globalSettings.roomId);
    }
}

void processDir(atomic<int>& delta, mhacks::Direction d)
{
    mhacks::motion m;
    if (d == mhacks::Right)
    {
        m = globalSettings.right;
    }
    else
    {
        m = globalSettings.left;
    }
    
    if (m == mhacks::Entry)
    {
        // add one to current counter
        delta++;
        cout << "Entry!" << endl;
    }
    else
    {
        // subtract one from counter
        delta--;
        cout << "Exit!" << endl;
    }
}

int main(int argc, char **argv)
{
    using namespace std::placeholders;
    if (!mhacks::load_settings(mhacks::get_default_filename(), globalSettings))
    {
        return 1;
    }

    RestClient::response r = mhacks::register_room(globalSettings.roomId);
    atomic<int> delta(0);
    if (r.code != 200)
    {
        cerr << "Registration with server failed." << endl;
        return 1;
    }

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

    auto procFunc = std::bind(processDir, std::ref(delta), _1);

    thread submittingThread(submitThread, std::ref(delta));

    mhacks::imgproc_loop(capture, procFunc, false);

    submittingThread.join();
    return 0;
}
