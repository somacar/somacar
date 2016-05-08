#include "target.h"

string toString(Point p) {
    stringstream ss;
    ss << p;
    return ss.str();
}

Target::Target() {}

void Target::init(Mat f, bool color) {
    f.copyTo(this->orig);
    if (!color) cvtColor(f, f, COLOR_RGB2GRAY);
    else {
        assert(f.type() == CV_8UC3);
        cvtColor(f, f, COLOR_BGR2HSV);
        inRange(f, LOWCOLOR, UPCOLOR, f);
//        mask = cv2.erode(mask, None, iterations=2)
//        mask = cv2.dilate(mask, None, iterations=2)
    }
    GaussianBlur(f, f, Size(7, 7), 0);
//    Canny(f, f, 50, 150);
    this->img = f;
}

void Target::found() {
    vector<vector<Point>> arr;
    arr.push_back(this->approx);

    drawContours(this->orig, arr, -1, DRAW, DRAW_THICK);

    Moments M = moments(this->approx);
    Point center = Point2f((int) (M.m10 / M.m00), (int) (M.m01 / M.m00));
    putText(this->orig, toString(center), center, FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);

    if (center.x < this->orig.size().width / 2) this->dir = LEFT;
    else this->dir = RIGHT;
}

bool Target::is_target(vector<Point> c) {
    if (this->approx.size() < 4 || this->approx.size() > 6) return false; // rectangular
    Rect rect = boundingRect(this->approx);
    if ((rect.width < 25) || (rect.height < 25)) return false; // min size 25px * 25px
    if ((rect.width / (float) rect.height < 0.8) || (rect.width / (float) rect.height > 1.2)) return false; // ratio
    vector<Point> hull;
    convexHull(c, hull);
    return ((contourArea(c) / (float) contourArea(hull)) > 0.9); // solidity
}

void Target::show() {
    string status;
    switch (this->dir) {
        case LEFT:
            status = "Go Right !";
            break;
        case RIGHT:
            status = "Go Left !";
            break;
    }
    putText(this->orig, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
    imshow("Original", this->orig);
//    imshow("Converted", this->img);
}

void Target::detect() {
    vector<vector<Point>> contours;
    findContours(this->img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (auto const &c: contours) {
        approxPolyDP(c, this->approx, 0.01 * arcLength(c, true), true);
        if (is_target(c)) found();
    }
}

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
        target.init(frame, true);
        target.detect();
        target.show();
        if ((char) waitKey(1) == 27) break;
    }
    return 0;
}