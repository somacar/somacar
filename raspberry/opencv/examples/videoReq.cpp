#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // 0

    Mat frame;
    const char win_name[]="Live Video...";
    int cam_id=0; // Webcam connected to the USB port
    double fps=20;
    char file_out[]="recorded.avi";

    if (argc == 2)
        sscanf(argv[1], "%d", &cam_id);

    VideoCapture inVid(cam_id); // Open camera with cam_id
    if (!inVid.isOpened()) {
        cout << "Error! Camera not ready...\n";
        return -1;
    }

    int width = (int)inVid.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)inVid.get(CV_CAP_PROP_FRAME_HEIGHT);
//    fps = inVid.get(CV_CAP_PROP_FPS); // Return 0 if not supported by the backend
    VideoWriter recVid(file_out, CV_FOURCC('F','F','D','S'), fps, Size(width, height));
    if (!recVid.isOpened()) {
        cout << "Error! Video file not opened...\n";
        return -1;
    }

    namedWindow(win_name); // Create a window for displaying the live video
    while (1) {
        inVid >> frame; // Read frame from camera
        recVid << frame; // Write frame to video file
        imshow(win_name, frame); // Show frame
        if (waitKey(1000/fps) >= 0)
            break;
    }
    inVid.release(); // Close camera
    return 0;
}