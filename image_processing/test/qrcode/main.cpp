#include <stdio.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include "qrReader.h"

#include "FinderPatternTrio.h"

using namespace cv;
using namespace std;

int main() {
    VideoCapture capture(0);
    assert(capture.isOpened());

    qrReader qr = qrReader();

//    Mat image = imread("QRCodeImg.jpg");
//    Mat imgBW = Mat(image.rows, image.cols, CV_8UC1);
    Mat image, imgBW;
    while(true) {
//        capture >> image;
        capture.read(image);
        imgBW = Mat(image.size(), CV_MAKETYPE(image.depth(), 1));
        cvtColor(image, imgBW, CV_BGR2GRAY);
        threshold(imgBW, imgBW, 128, 255, THRESH_BINARY);

        qr.setImage(imgBW);
        FinderPatternTrio *trio = qr.find();

        FinderPattern *topLeft = trio->getTopLeft();
        FinderPattern *topRight = trio->getTopRight();
        FinderPattern *bottomLeft= trio->getBottomLeft();

        line(image, Point(topLeft->getX(), topLeft->getY()), Point(topRight->getX(), topRight->getY()), CV_RGB(255,0,0), 2);
        line(image, Point(topLeft->getX(), topLeft->getY()), Point(bottomLeft->getX(), bottomLeft->getY()), CV_RGB(255,255,0), 2);

        imshow("Original", image);
        imshow("QR Code", imgBW);
        if ((char)waitKey(20)==27) break;
    }

//    waitKey(0);
    return 0;
}
