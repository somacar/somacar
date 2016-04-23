#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

String face_cas = "face.xml";
String eye_cas = "eye.xml";
CascadeClassifier face, eye;

void FaceAndEyeDetect(Mat);

int main(int argc, char *argv[]) {
    Mat f = imread("scene.jpg");
    assert(f.data);

    bool b1 = face.load(face_cas);
    bool b2 = eye.load(eye_cas);
    assert(b1 && b2);
    FaceAndEyeDetect(f);

    waitKey(0);
    return 0;
}

void FaceAndEyeDetect(Mat img) {
    Mat gray;
    cvtColor(img, gray, CV_BGR2GRAY);

    std::vector<Rect> face_pos;
    face.detectMultiScale(gray, face_pos, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

    for (int i = 0; i < face_pos.size(); i++) {
        rectangle(img, face_pos[i], Scalar(255, 0, 0), 2);
    }

    for (int i = 0; i < face_pos.size(); i++) {
        std::vector<Rect> eye_pos;
        Mat roi = gray(face_pos[i]);

        eye.detectMultiScale(roi, eye_pos, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

        for (int j = 0; j < eye_pos.size(); j++) {
            Point center(face_pos[i].x + eye_pos[j].x + eye_pos[j].width * 0.5,
                         face_pos[i].y + eye_pos[j].y + eye_pos[j].height * 0.5);
            int radius = cvRound((eye_pos[j].width + eye_pos[j].height) * 0.25);
            circle(img, center, radius, Scalar(0, 0, 255), 2, 8, 0);
        }
    }

    namedWindow("얼굴 검출");
    imshow("얼굴 검출", img);
}