#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#define RED Scalar(0, 0, 255)

Mat img1;

void detect(Mat);
void drawRect(Mat, vector<DMatch>, vector<KeyPoint>, vector<KeyPoint>);

int main(int argc, char *argv[]) {
    Mat img1 = imread("stop.png", IMREAD_GRAYSCALE);
    Mat img2 = imread("scene.jpg", IMREAD_GRAYSCALE);

    if (!img1.data || !img2.data) {
        cerr << " Failed to load images." << endl;
        return -1;
    }

    detect(img2);
    waitKey(0);
    return 0;
}

void detect(Mat f) {
    Mat img2;
    cvtColor(f, img2, CV_BGR2GRAY);
    Ptr<FeatureDetector> detector = AKAZE::create();
    vector<KeyPoint> keypoints1, keypoints2;

    detector->detect(img1, keypoints1);
    detector->detect(img2, keypoints2);

    Ptr<DescriptorExtractor> extractor = AKAZE::create();
    Mat descriptors1, descriptors2;

    extractor->compute(img1, keypoints1, descriptors1);
    extractor->compute(img2, keypoints2, descriptors2);

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    vector<vector<DMatch> > matches;
    vector<DMatch> good_matches;

    matcher->knnMatch(descriptors1, descriptors2, matches, 2);

    float ratioT = 0.6;
    for (int i = 0; i < (int) matches.size(); i++) {
        if ((matches[i][0].distance < ratioT * (matches[i][1].distance)) &&
            ((int) matches[i].size() <= 2 && (int) matches[i].size() > 0)) {
            good_matches.push_back(matches[i][0]);
        }
    }

    drawRect(f, good_matches, keypoints1, keypoints2);
}

void drawRect(Mat f, vector<DMatch> good_matches, vector<KeyPoint> key1, vector<KeyPoint> key2) {
    vector<Point2f> model_pt, scene_pt;
    for (int i = 0; i < good_matches.size(); i++) {
        model_pt.push_back(key1[good_matches[i].queryIdx].pt);
        scene_pt.push_back(key2[good_matches[i].trainIdx].pt);
    }

    Mat H = findHomography(model_pt, scene_pt, CV_RANSAC);

    vector<Point2f> model_corner(4);
    model_corner[0] = cvPoint(0, 0);
    model_corner[1] = cvPoint(img1.cols, 0);
    model_corner[2] = cvPoint(img1.cols, img1.rows);
    model_corner[3] = cvPoint(0, img1.rows);

    vector<Point2f> scene_corner(4);
    perspectiveTransform(model_corner, scene_corner, H);

    line(f, scene_corner[0], scene_corner[1], RED, 3);
    line(f, scene_corner[1], scene_corner[2], RED, 3);
    line(f, scene_corner[2], scene_corner[3], RED, 3);
    line(f, scene_corner[3], scene_corner[0], RED, 3);

    namedWindow("Matching AKAZE");
    imshow("Matching AKAZE Descriptors", f);
}