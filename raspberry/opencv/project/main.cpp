#include "main.hpp"
#include "target.hpp"
#include "ocr.hpp"

using namespace cv;

int main(int argc, char *argv[]) {
    VideoCapture v(0);
    Mat frame;
    UMat sqr;
    Target target;
    OCRTess tess(true, REG_MSER, GR_EX);
    tess.init(10);

//    frame = imread("sam.jpg");
//    target.init(frame);
//    target.detect();
//    target.show();
//    waitKey(0);

    while (true) {
        v.read(frame);
        target.init(frame.getUMat(ACCESS_READ));
        if (target.find_square(&sqr)) {
            tess.set(sqr);
            tess.detectAndRecog();
            tess.show();
        }
        target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}