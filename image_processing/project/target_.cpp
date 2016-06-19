#include "main.hpp"
#include "target.hpp"

int sp, pre_status = STOP, stop_once = false;

Target::Target() {
//    sp = serialOpen("/dev/ttyACM0", 9600);
}

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

bool Target::is_square(vector <Point> c, Rect *rect) {
    if (this->approx.size() != 4) return false; // rectangular
    *rect = boundingRect(this->approx);
    if ((rect->width < MIN_RECT) || (rect->height < MIN_RECT)) return false; // min size
    this->dist = ORIG_WIDTH * ORIG_F / rect->width;
    this->out_width = rect->width;
    if (rect->width / (float) rect->height < 0.8 || rect->width / (float) rect->height > 1.2) return false; // ratio
    vector <Point> hull;
    convexHull(c, hull);
    return ((contourArea(c) / (float) contourArea(hull)) > 0.9);
}

bool Target::is_rect(vector <Point> c) {
    if (c.size() != 4) return false; // rectangular
    Rect rect = boundingRect(c);
    if ((rect.width < MIN_RECT) || (rect.height < MIN_RECT)) return false; // min size
    if (rect.width / (float) rect.height < 0.8 || rect.width / (float) rect.height > 1.2) return false; // ratio
    if (this->out_width / (float) rect.width < 0.8 || this->out_width / (float) rect.width > 1.2) return false; // ratio
    vector <Point> hull;
    convexHull(c, hull);
    return ((contourArea(c) / (float) contourArea(hull)) > 0.9);
}

bool Target::is_inside(UMat u) {
    vector<Vec3f> circles;
    HoughCircles(c, circles, HOUGH_GRADIENT, 1, 10,
                 100, 30, 1, 30 // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( this->draw, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( this->draw, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }

    return (circles.size() == 1);

    bitwise_not(u, u);
    vector <vector<Point>> contours;
    findContours(u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (auto const &c: contours)
        if (is_rect(c)) return true;
//    if (count == 2) return true;
    return false;
}

bool Target::is_star(UMat u) {
    double divMaxSize = 0.175, divMinSize = 0.125;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
//    cvtColor(u, u, COLOR_RGB2GRAY);
    threshold(u, u, 100, 255, 0);
    findContours(u, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (int i=0; i < contours.size(); i++) {
        cout << to_string(sqrt(contourArea(contours[i]))/arcLength(contours[i], true )) << "star;;;;;" << endl;
        if(sqrt(contourArea(contours[i]))/arcLength(contours[i], true ) < divMaxSize && sqrt(contourArea(contours[i]))/arcLength(contours[i], true) > divMinSize) {
            drawContours(this->draw, contours, i, DRAW, 2, 8, hierarchy, 0, Point());
            return true;
        }
    }
    return false;
}

int Target::found(bool b) {
    if (!b) {
        return STOP;
    }
    vector <vector<Point>> arr;
    arr.push_back(this->approx);
    Moments M = moments(this->approx);
    Point center = Point2f((int) (M.m10 / M.m00), (int) (M.m01 / M.m00));

    int status = STOP;
    if (center.x <= (this->orig.size().width / 2) - (this->orig.size().width / (DEST_ST * 2))) status += LEFT;
    else if (center.x >= (this->orig.size().width / 2) + (this->orig.size().width / (DEST_ST * 2))) status += RIGHT;
    else status += CENTER;
    drawContours(this->draw, arr, -1, DRAW, DRAW_THICK);
    return status;
    //putText(this->draw, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.5, DRAW, DRAW_THICK);
}

void Target::serial(int result) {
    if (result != STOP) stop_once = false;
    else {
        if (!stop_once) {
            stop_once = true;
            return;
        } else {
//            serialPutchar(sp, (char) STOP);
        }
    }
    if (pre_status == result) return;
//    serialPutchar(sp, (char) result);
    switch (result) {
        case CENTER:
            cout << "center" << endl;
            break;
        case LEFT:
            cout << "left" << endl;
            break;
        case RIGHT:
            cout << "right" << endl;
            break;
    }
    pre_status = result;
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
            warpPerspective(this->orig, *sqr, tr, sqr->size());
            return true;
        }
    }
    return false;
}
