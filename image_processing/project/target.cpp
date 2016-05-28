#include "main.hpp"
#include "target.hpp"

string toString(Point p) {
    stringstream ss;
    ss << p;
    return ss.str();
}

Target::Target() { }

void Target::init(UMat f, bool color) {
    f.copyTo(this->orig);
    f.copyTo(this->draw);
    if (!color) cvtColor(f, f, COLOR_RGB2GRAY);
    else {
        assert(f.type() == CV_8UC3);
        cvtColor(f, f, COLOR_BGR2HSV);
        inRange(f, LOWCOLOR, UPCOLOR, f);
//        mask = cv2.erode(mask, None, iterations=2)
//        mask = cv2.dilate(mask, None, iterations=2)
    }
    GaussianBlur(f, f, Size(7, 7), 0);
    Canny(f, f, 50, 150);
    this->cvt = f;
}

void Target::found() {
    vector<vector<Point>> arr;
    arr.push_back(this->approx);

    drawContours(this->draw, arr, -1, DRAW, DRAW_THICK);

    Moments M = moments(this->approx);
    Point center = Point2f((int) (M.m10 / M.m00), (int) (M.m01 / M.m00));
    putText(this->draw, toString(center), center, FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);

    for (int i = 0; i < this->approx.size(); i++) {
        putText(this->draw, toString(this->approx[i]), this->approx[i], FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
    }

    if (center.x < this->orig.size().width / 2) this->dir = LEFT;
    else this->dir = RIGHT;
}

bool Target::is_square(vector<Point> c, Rect *rect) {
    if (this->approx.size() != 4) return false; // rectangular
    *rect = boundingRect(this->approx);
    if ((rect->width < MIN_RECT) || (rect->height < MIN_RECT)) return false; // min size 25px * 25px
    this->dist = ORIG_WIDTH * ORIG_P / rect->width;
//    if ((rect->width / (float) rect->height < 0.8) || (rect->width / (float) rect->height > 1.2)) return false; // ratio
    vector<Point> hull;
    convexHull(c, hull);
    return ((contourArea(c) / (float) contourArea(hull)) > 0.9);
}

void Target::show() {
    string status;
    switch (this->dir) {
        case LEFT:
            status = "Go Right !";
            break;
        case RIGHT:
        default:
            status = "Go Left !";
            break;
    }
    putText(this->draw, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
    putText(this->draw, to_string(this->dist) + " cm", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
    imshow("Original", this->draw);
}

bool Target::find_square(UMat *sqr) {
    vector<vector<Point>> contours;
    Rect rect;
    findContours(this->cvt, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (auto const &c: contours) {
        approxPolyDP(c, this->approx, 0.01 * arcLength(c, true), true);
        if (is_square(c, &rect)) {
            found();
            *sqr = this->orig(rect);

            vector<Point2f> corn_pt, quad_pt;
            for (int i = 0; i < this->approx.size(); i++)
                corn_pt.push_back(Point2f(this->approx[i].x, this->approx[i].y));
            quad_pt.push_back(Point2f(0, 0));
            quad_pt.push_back(Point2f(0, rect.height));
            quad_pt.push_back(Point2f(rect.width, rect.height));
            quad_pt.push_back(Point2f(rect.width, 0));

            UMat tr = getPerspectiveTransform(corn_pt, quad_pt).getUMat(ACCESS_READ);
            warpPerspective(this->orig.clone(), *sqr, tr, sqr->size());
            return true;
        }
    }
    return false;
}