#include "target.h"

using namespace cv;

int main(int argc, char *argv[]) {
//    VideoCapture v("FlightDemo.mp4");
    VideoCapture v(0);
    Mat frame;
    Target target;

//    frame = imread("sam.jpg");
//    target.init(frame);
//    target.detect();
//    target.show();
//    waitKey(0);

    while (true) {
        v.read(frame);
        target.init(frame.getUMat(ACCESS_READ));
        target.detect();
        target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}