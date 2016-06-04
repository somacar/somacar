#include "main.hpp"
#include "target.hpp"

string toString(Point p) {
    stringstream ss;
    ss << p;
    return ss.str();
}

Target::Target() { }

void Target::init(UMat u, bool color) {
    this->orig = u.clone();
    this->draw = u.clone();
    if (!color) {
        cvtColor(u.clone(), u, COLOR_RGB2GRAY);
        //threshold(u, u, 128, 255, THRESH_BINARY | THRESH_OTSU);
//        inRange(f, Scalar(0, 0, 0, 0), Scalar(160, 255, 30, 0), f);
    } else {
        cvtColor(u, u, COLOR_BGR2HSV);
        inRange(u, LOWCOLOR, UPCOLOR, u);
//        mask = cv2.erode(mask, None, iterations=2)
//        mask = cv2.dilate(mask, None, iterations=2)
    }
    GaussianBlur(u, u, Size(7, 7), 1.5, 1.5);
    Canny(u, u, 50, 150);
    this->cvt = u;
}

bool Target::is_square(vector<Point> c, Rect *rect) {
    if (this->approx.size() != 4) return false; // rectangular
    *rect = boundingRect(this->approx);
    if ((rect->width < MIN_RECT) || (rect->height < MIN_RECT)) return false; // min size
    this->dist = ORIG_WIDTH * ORIG_F / rect->width;
    if (rect->width / (float) rect->height < 0.8 || rect->width / (float) rect->height > 1.2) return false; // ratio
    vector<Point> hull;
    convexHull(c, hull);
    return ((contourArea(c) / (float) contourArea(hull)) > 0.9);
}

void Target::found_square() {
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

void Target::found_word(bool b) {
    if (!b) return;
    cout << "found word !!!!!!!!!!!!!!!" << endl;
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
}

void Target::show() {
    imshow("target", this->draw);
}

bool Target::find_square(UMat *sqr) {
    vector<vector<Point>> contours;
    Rect rect;
    findContours(this->cvt, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (auto const &c: contours) {
        approxPolyDP(c, this->approx, 0.01 * arcLength(c, true), true);
        if (is_square(c, &rect)) {
            found_square();
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