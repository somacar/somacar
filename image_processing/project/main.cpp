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
    bool found = false;

    assert(v.isOpened());
    tess.init(10);

//    frame = imread("test.jpg");
//    target.init(frame.getUMat(ACCESS_READ));
//    if (target.find_square(&sqr)) {
//        tess.set(sqr);
//        found = tess.loop();
//        tess.show(found);
//    }
//    target.found_word(found);
//    target.show();
//    waitKey(0);

    ocl::setUseOpenCL(true);

    while (true) {
        v.read(frame);
        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        target.init(frame.getUMat(ACCESS_READ));
        if (target.find_square(&sqr)) {
            tess.set(sqr);
            found = tess.loop();
            tess.show(found);
        }
        target.found_word(found);
        target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}