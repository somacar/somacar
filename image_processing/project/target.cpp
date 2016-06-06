#include "main.hpp"
#include "target.hpp"

RNG rngee(12345);

Target::Target() { }

void Target::init(UMat u) {
    this->draw = u.clone();
    cvtColor(u.clone(), u, COLOR_RGB2GRAY);
    this->orig = u.clone();
    //threshold(u, u, 128, 255, THRESH_BINARY | THRESH_OTSU);
    //inRange(f, Scalar(0, 0, 0, 0), Scalar(160, 255, 30, 0), f);
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

bool Target::is_star(UMat u) {
    double divMaxSize = 0.175, divMinSize = 0.125;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    double c;
//
//    cvtColor(u, u, CV_RGB2GRAY);
    threshold(u, u, 100, 255, 0);
//
    findContours(u, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
    for (int i=0; i < contours.size(); i++) {
        Scalar color = Scalar( rngee.uniform(0, 255), rngee.uniform(0,255), rngee.uniform(0,255) );
        c = sqrt(contourArea(contours[i]))/arcLength(contours[i], true );
        cout << "sqrt(Area)/arcLength = " << c << endl;
        if(sqrt(contourArea(contours[i]))/arcLength(contours[i], true ) < divMaxSize && sqrt(contourArea(contours[i]))/arcLength(contours[i], true) > divMinSize)
        {
            drawContours(this->draw, contours, i, color, 2, 8, hierarchy, 0, Point() );
            cout << "I'm a star!" << endl;
            return true;
        }
    }
    return false;
}

void Target::found(bool b) {
    if (!b) return;
    cout << "found!!!!!!!!!!!!!!!!!!" << endl;
    vector<vector<Point>> arr;
    arr.push_back(this->approx);
    Moments M = moments(this->approx);
    Point center = Point2f((int) (M.m10 / M.m00), (int) (M.m01 / M.m00));
    int dir = CENTER;
    if (center.x < this->orig.size().width / 2) dir = LEFT;
    if (center.x > this->orig.size().width / 6) dir = RIGHT;
    switch (dir) {
        case LEFT:
            cout << "left" << endl;
            break;
        case RIGHT:
            cout << "right" << endl;
            break;
        default:
            cout << "go" << endl;
            break;
    }

    drawContours(this->draw, arr, -1, DRAW, DRAW_THICK);
//    line(this->draw, Point(center.x, center.y - DRAW_CROSS), Point(center.x, center.y + DRAW_CROSS), DRAW, DRAW_THICK);
//    line(this->draw, Point(center.x - DRAW_CROSS, center.y), Point(center.x + DRAW_CROSS, center.y), DRAW, DRAW_THICK);
//    putText(this->draw, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
//    putText(this->draw, to_string(this->dist) + " cm", Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
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
            this->c = c;
            return true;
        }
    }
    return false;
}