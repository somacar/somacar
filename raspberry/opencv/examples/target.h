//
// Created by 이영인 on 2016. 5. 5..
//

#ifndef OPENCV_TARGET_H
#define OPENCV_TARGET_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define LEFT 0
#define RIGHT 1

#define DRAW Scalar(0, 0, 255)
#define DRAW_THICK 1
#define LOWCOLOR Scalar(0, 200, 0)
#define UPCOLOR Scalar(19, 255, 255)

using namespace cv;
using namespace std;

int DIRECTION;

class Target {
public:
    Target();
    void init(Mat f, bool color=false);
    void detect();
    bool is_target(vector<Point> c);
    void found();
    void show();
private:
    Mat orig, img;
    int dir;
    vector<Point> approx;
};

#endif
