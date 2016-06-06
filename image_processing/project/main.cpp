#include "main.hpp"
#include "target.hpp"
#include "ocr.hpp"

using namespace cv;

int main(int argc, char *argv[]) {
    VideoCapture v(0);
    Mat frame;
    UMat sqr;
    Target target;
    OCRTess tess;
    int mode = TARGET_STAR;
    bool found;

    wiringPiSetup();

    if (mode == TARGET_TEXT) {
        tess = OCRTess(true, REG_MSER, GR_EX);
        tess.init(10);
    }

    assert(v.isOpened());

    while (true) {
        v.read(frame);
        resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
        target.init(frame.getUMat(ACCESS_READ));
        if (target.find_square(&sqr)) {
            if (mode == TARGET_TEXT) {
                tess.set(sqr);
                found = tess.loop();
                tess.show(found);
            } else {
                found = target.is_star(sqr);
            }
            target.found(found);
        }
        target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}