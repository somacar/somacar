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
    bool found;


    if (MODE == TARGET_TEXT) {
        tess = OCRTess(true, REG_MSER, GR_EX);
        tess.init(10);
    }

    if (!v.isOpened()) {
        printf("ERR: camera is not opened !!\n\n");
        return 0;
    }

    while (true) {
        v.read(frame);
        resize(frame, frame, Size(frame.cols / 2, frame.rows / 2));
        target.init(frame.getUMat(ACCESS_READ));
        if (target.find_square(&sqr)) {
            if (MODE == TARGET_TEXT) {
                tess.set(sqr);
                found = tess.loop();
                tess.show(found);
            } else {
                found = target.is_inside(sqr);
            }
            target.found(found);
	    target.serial();
        }
        //target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}
