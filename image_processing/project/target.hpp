//
// Created by 이영인 on 2016. 5. 5..
//

#ifndef OPENCV_TARGET_H
#define OPENCV_TARGET_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define CENTER 0
#define LEFT 1
#define RIGHT 2

#define ORIG_WIDTH 14
#define ORIG_DIS 73
#define ORIG_PIXEL 196
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

    bool is_star(UMat u);

    void found(bool b);

    void show();

private:
    UMat orig, cvt, draw;
    int dist;
    vector<Point> approx, c;
};

#endif
