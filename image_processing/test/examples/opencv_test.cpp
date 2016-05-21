#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    cout << "OpenCV Version : " << CV_VERSION << endl;
    Mat img;
    namedWindow("EXAMPLE01", CV_WINDOW_AUTOSIZE);

    img = imread("test.jpg", CV_LOAD_IMAGE_COLOR); // 불러올 파일 이름
    if (img.empty()) // 파일이 없을 경우 에러메세지 출력
    {
        cout << "[!] Image load fail!" << endl;
        return -1;
    }
    imshow("EXAMPLE01", img);
    waitKey(0);
    destroyWindow("EXAMPLE01");
    return 0;
}