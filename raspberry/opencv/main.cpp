#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/flann/flann.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

#define RED Scalar(0, 0, 255)

int main(int argc, char *argv[]) {
    Mat img1 = imread("stop.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat img2 = imread("scene.JPG", CV_LOAD_IMAGE_GRAYSCALE);
    assert(img1.data && img2.data);

    cv::Ptr<Feature2D> detector = xfeatures2d::SURF::create(0.3);
    std::vector<KeyPoint> key1, key2;

    detector->detect(img1, key1);
    detector->detect(img2, key2);

    Mat disp;
    drawKeypoints(img1, key1, disp, RED, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    namedWindow("키포인트");
    imshow("키포인트", disp);

    cv::Ptr<Feature2D> extractor = xfeatures2d::SURF::create();
    Mat des1, des2;
    extractor->compute(img1, key1, des1);
    extractor->compute(img2, key2, des2);

    FlannBasedMatcher matcher;
    std::vector<DMatch> match;
    matcher.match(des1, des2, match);

    double maxd = 0;
    double mind = 100;
    for (int i = 0; i < des1.rows; i++) {
        double dist = match[i].distance;
        if (dist < mind) mind = dist;
        if (dist > maxd) maxd = dist;
    }

    std::vector<DMatch> good_match;
    for (int i = 0; i < des1.rows; i++)
        if (match[i].distance <= max(2 * mind, 0.02)) good_match.push_back(match[i]);

    Mat img_match;
    drawMatches(img1, key1, img2, key2, good_match, img_match, Scalar::all(-1), Scalar::all(-1), std::vector<char>(),
                DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    namedWindow("매칭 결과");
    imshow("매칭 결과", img_match);

    for (int i = 0; i < (int) good_match.size(); i++)
        printf("키포인트 %d~%d\n", good_match[i].queryIdx, good_match[i].trainIdx);

    std::vector<Point2f> model_pt;
    std::vector<Point2f> scene_pt;
    for (int i = 0; i < good_match.size(); i++) {
        model_pt.push_back(key1[good_match[i].queryIdx].pt);
        scene_pt.push_back(key2[good_match[i].trainIdx].pt);
    }

    Mat H = findHomography(model_pt, scene_pt, CV_RANSAC);

    std::vector<Point2f> model_corner(4);
    model_corner[0] = cvPoint(0, 0);
    model_corner[1] = cvPoint(img1.cols, 0);
    model_corner[2] = cvPoint(img1.cols, img1.rows);
    model_corner[3] = cvPoint(0, img1.rows);

    std::vector<Point2f> scene_corner(4);
    perspectiveTransform(model_corner, scene_corner, H);

    Point2f p(img1.cols, 0);
    line(img_match, scene_corner[0]+p, scene_corner[1]+p, RED, 3);
    line(img_match, scene_corner[1]+p, scene_corner[2]+p, RED, 3);
    line(img_match, scene_corner[2]+p, scene_corner[3]+p, RED, 3);
    line(img_match, scene_corner[3]+p, scene_corner[0]+p, RED, 3);

    waitKey(0);
    return 0;
}