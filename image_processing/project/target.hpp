#ifndef OPENCV_TARGET_H
#define OPENCV_TARGET_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <wiringSerial.h>

#define STOP 0
#define CENTER 1
#define LEFT 2
#define RIGHT 3

#define DEST_ST 6

#define MIN_DIST 50

#define ORIG_WIDTH 14
#define ORIG_DIS 30
#define ORIG_PIXEL 192
#define ORIG_F (ORIG_PIXEL * ORIG_DIS / ORIG_WIDTH)

#define LOWCOLOR Scalar(0, 200, 0)
#define UPCOLOR Scalar(19, 255, 255)

using namespace cv;
using namespace std;

class Target {
public:
    Target();

    void init(UMat u);

    bool find_square(UMat *sqr);

    bool is_square(vector<Point> c, Rect *rect);

    bool is_rect(vector<Point> c);

    bool is_inside(UMat u);

    int found(bool b);

    void serial(int r);

    void show();

private:
    UMat orig, cvt, draw;
    int dist, out_width;
    vector<Point> approx;
};

#endif
