#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

#include<iostream>
#include<vector>

using namespace cv;
using namespace std;

// global variables ///////////////////////////////////////////////////////////////////////////////
const int MIN_CONTOUR_AREA = 100;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

    Mat imgTrainingNumbers;         // input image
    Mat imgGrayscale;               // 
    Mat imgBlurred;                 // declare various images
    Mat imgThresh;                  //
    Mat imgThreshCopy;              //

    vector<vector<Point> > ptContours;        // declare contours vector
    vector<Vec4i> v4iHierarchy;                    // declare contours hierarchy

    Mat matClassificationInts;      // these are our training classifications, note we will have to perform some conversions before writing to file later

    // these are our training images, due to the data types that the KNN object KNearest requires, we have to declare a single Mat,
    // then append to it as though it's a vector, also we will have to perform some conversions before writing to file later
    Mat matTrainingImagesAsFlattenedFloats;

    // possible chars we are interested in are digits 0 through 9 and capital letters A through Z, put these in vector intValidChars
    vector<int> intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                                       'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                                       'U', 'V', 'W', 'X', 'Y', 'Z' };

    imgTrainingNumbers = imread("training_chars.png");          // read in training numbers image

    if (imgTrainingNumbers.empty()) {                               // if unable to open image
        cout << "error: image not read from file\n\n";         // show error message on command line
        return(0);                                                  // and exit program
    }

    cvtColor(imgTrainingNumbers, imgGrayscale, CV_BGR2GRAY);        // convert to grayscale

    GaussianBlur(imgGrayscale,              // input image
                     imgBlurred,                             // output image
                     Size(5, 5),                         // smoothing window width and height in pixels
                     0);                                     // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

    // filter image from grayscale to black and white
    adaptiveThreshold(imgBlurred,           // input image
                          imgThresh,                              // output image
                          255,                                    // make pixels that pass the threshold full whiteZ
                          ADAPTIVE_THRESH_GAUSSIAN_C,         // use gaussian rather than mean, seems to give better results
                          THRESH_BINARY_INV,                  // invert so foreground will be white, background will be black
                          11,                                     // size of a pixel neighborhood used to calculate threshold value
                          2);                                     // constant subtracted from the mean or weighted mean

    imshow("imgThresh", imgThresh);         // show threshold image for reference

    imgThreshCopy = imgThresh.clone();          // make a copy of the thresh image, this in necessary b/c findContours modifies the image

    findContours(imgThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
                     ptContours,                             // output contours
                     v4iHierarchy,                           // output hierarchy
                     RETR_EXTERNAL,                      // retrieve the outermost contours only
                     CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

    for (int i = 0; i < ptContours.size(); i++) {                           // for each contour
        if (contourArea(ptContours[i]) > MIN_CONTOUR_AREA) {                // if contour is big enough to consider
            Rect boundingRect = boundingRect(ptContours[i]);                // get the bounding rect

            rectangle(imgTrainingNumbers, boundingRect, Scalar(0, 0, 255), 2);      // draw red rectangle around each contour as we ask user for input

            Mat matROI = imgThresh(boundingRect);           // get ROI image of bounding rect

            Mat matROIResized;
            resize(matROI, matROIResized, Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

            imshow("matROI", matROI);                               // show ROI image for reference
            imshow("matROIResized", matROIResized);                 // show resized ROI image for reference
            imshow("imgTrainingNumbers", imgTrainingNumbers);       // show training numbers image, this will now have red rectangles drawn on it

            int intChar = waitKey(0);           // get key press

            if (intChar == 27) {        // if esc key was pressed
                return(0);              // exit program
            } else if (find(intValidChars.begin(), intValidChars.end(), intChar) != intValidChars.end()) {     // else if the char is in the list of chars we are looking for . . .

                matClassificationInts.push_back(intChar);       // append classification char to integer list of chars

                Mat matImageFloat;                          // now add the training image (some conversion is necessary first) . . .
                matROIResized.convertTo(matImageFloat, CV_32FC1);       // convert Mat to float

                Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);       // flatten

                matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);       // add to Mat as though it was a vector, this is necessary due to the
                // data types that KNearest.train accepts
            }   // end if
        }   // end if
    }   // end for

    cout << "training complete\n\n";

    // save classifications to file ///////////////////////////////////////////////////////

    FileStorage fsClassifications("classifications.xml", FileStorage::WRITE);           // open the classifications file

    if (fsClassifications.isOpened() == false) {                                                        // if the file was not opened successfully
        cout << "error, unable to open training classifications file, exiting program\n\n";        // show error message
        return(0);                                                                                      // and exit program
    }

    fsClassifications << "classifications" << matClassificationInts;        // write classifications into classifications section of classifications file
    fsClassifications.release();                                            // close the classifications file

    // save training images to file ///////////////////////////////////////////////////////

    FileStorage fsTrainingImages("images.xml", FileStorage::WRITE);         // open the training images file

    if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
        cout << "error, unable to open training images file, exiting program\n\n";         // show error message
        return(0);                                                                              // and exit program
    }

    fsTrainingImages << "images" << matTrainingImagesAsFlattenedFloats;         // write training images into images section of images file
    fsTrainingImages.release();                                                 // close the training images file

    return(0);
}