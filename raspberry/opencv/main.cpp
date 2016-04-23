#include <stdio.h>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qrReader.h"

using namespace std;
using namespace cv;

int main() {
    VideoCapture v(0);
    assert(v.isOpened());

    qrReader qr = qrReader();
    Mat frame, gray;
    while(true) {
        v.read(frame);

        cvtColor(frame, gray, CV_BGR2GRAY);
        threshold(gray, gray, 128, 255, THRESH_BINARY);

        bool found = qr.find(gray);
        if(found)
            qr.draw(gray);
    }
}