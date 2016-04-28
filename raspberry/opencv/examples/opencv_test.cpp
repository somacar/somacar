#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    std::cout << "OpenCV Version : " << CV_VERSION << std::endl;
    cv::Mat img;
    cv::namedWindow("EXAMPLE01", CV_WINDOW_AUTOSIZE);

    img = cv::imread("test.jpg", CV_LOAD_IMAGE_COLOR); // 불러올 파일 이름
    if (img.empty()) // 파일이 없을 경우 에러메세지 출력
    {
        std::cout << "[!] Image load fail!" << std::endl;
        return -1;
    }
    cv::imshow("EXAMPLE01", img);
    cv::waitKey(0);
    cv::destroyWindow("EXAMPLE01");
    return 0;
}