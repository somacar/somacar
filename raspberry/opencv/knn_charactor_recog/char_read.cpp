#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

#include<iostream>
#include<sstream>

using namespace std;
using namespace cv;

const int MIN_CONTOUR_AREA = 100;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

Ptr<ml::KNearest> kNearest;

class ContourWithData {
public:
    vector<Point> ptContour;           // contour
    Rect boundingRect;                      // bounding rect for contour
    float fltArea;                              // area of contour

    bool checkIfContourIsValid() {                              // obviously in a production grade program
        if (fltArea < MIN_CONTOUR_AREA) return false;           // we would have a much more robust function for
        return true;                                            // identifying if a contour is valid !!
    }

    static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
        return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
    }

};

int train() {
    Mat matClassificationInts;      // read the classification numbers into this variable as a vector

    FileStorage fsClassifications("classifications.xml", FileStorage::READ);

    if (!fsClassifications.isOpened()) {
        cout << "error, unable to open training classifications file, exiting program\n\n";
        return false;
    }

    fsClassifications["classifications"] >> matClassificationInts;
    fsClassifications.release();

    Mat matTrainingImagesAsFlattenedFloats; // read multiple images into single image variable as a vector

    FileStorage fsTrainingImages("images.xml", FileStorage::READ);

    if (!fsTrainingImages.isOpened()) {
        cout << "error, unable to open training images file, exiting program\n\n";
        return false;
    }

    fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats; // read images section into Mat training images variable
    fsTrainingImages.release();

    kNearest = ml::KNearest::create();            // instantiate the KNN object

    // finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
    // even though in reality they are multiple images / numbers
    kNearest->train(matTrainingImagesAsFlattenedFloats, ml::ROW_SAMPLE, matClassificationInts);
    return true;
}

int read(Mat img) {
    if (img.empty()) {
        cout << "error: image not read from file\n\n";
        return false;
    }

    vector<ContourWithData> allContoursWithData;
    vector<ContourWithData> validContoursWithData;

    Mat matGrayscale;
    Mat matBlurred;
    Mat matThresh;
    Mat matThreshCopy;

    cvtColor(img, matGrayscale, CV_BGR2GRAY);
    GaussianBlur(matGrayscale, matBlurred, Size(5, 5), 0);

    // filter image from grayscale to black and white
    adaptiveThreshold(matBlurred, matThresh,
                      255,                              // make pixels that pass the threshold full white
                      ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
                      THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
                      11,                               // size of a pixel neighborhood used to calculate threshold value
                      2);                               // constant subtracted from the mean or weighted mean

    matThreshCopy = matThresh.clone();  // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    vector<vector<Point> > ptContours;
    vector<Vec4i> v4iHierarchy;

    findContours(matThreshCopy, ptContours, v4iHierarchy,
                 RETR_EXTERNAL,                      // retrieve the outermost contours only
                 CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

    for (int i = 0; i < ptContours.size(); i++) {
        ContourWithData contourWithData;                                            // instantiate a contour with data object
        contourWithData.ptContour = ptContours[i];                                  // assign contour to contour with data
        contourWithData.boundingRect = boundingRect(contourWithData.ptContour);     // get the bounding rect
        contourWithData.fltArea = contourArea(contourWithData.ptContour);           // calculate the contour area
        allContoursWithData.push_back(contourWithData);                             // add contour with data object to list of all contours with data
    }

    for (int i = 0; i < allContoursWithData.size(); i++) {              // for all contours
        if (allContoursWithData[i].checkIfContourIsValid()) {           // check if valid
            validContoursWithData.push_back(allContoursWithData[i]);    // if so, append to valid contour list
        }
    }
    // sort contours from left to right
    sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

    string strFinalString;         // declare final string, this will have the final number sequence by the end of the program

    for (int i = 0; i < validContoursWithData.size(); i++) {
        rectangle(img,                                      // draw rectangle on original image
                  validContoursWithData[i].boundingRect,    // rect to draw
                  Scalar(0, 255, 0),                        // green
                  2);                                       // thickness

        Mat matROI = matThresh(validContoursWithData[i].boundingRect);          // get ROI image of bounding rect

        Mat matROIResized;
        resize(matROI, matROIResized, Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT)); // resize image, this will be more consistent for recognition and storage

        Mat matROIFloat;
        matROIResized.convertTo(matROIFloat, CV_32FC1); // convert Mat to float, necessary for call to find_nearest

        Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

        Mat matCurrentChar(0, 0, CV_32F);

        kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);

        float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

        strFinalString = strFinalString + char(int(fltCurrentChar));
    }

    cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";

    imshow("matTestingNumbers", img);

    waitKey(0);
}

int main() {
    if (!train())
        return (0);

    VideoCapture v(0);
    assert(v.isOpened());

    Mat frame = imread("test1.png");
    if (!read(frame))
        return (0);

//    Mat frame;
//    while(true) {
//        v.read(frame);
//        if (!read(frame)) break;
//        if ((char)waitKey(20)==27) break;
//    }

    return(0);
}