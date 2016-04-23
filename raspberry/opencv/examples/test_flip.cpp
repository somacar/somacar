#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // -1 test.jpg test_f.jpg

    int flip_code=0;
    const char win[]="Flipped...";
    Mat out_image; // Output image

    if (argc != 4) {//Check args. number
        cout << "Usage: <cmd> <flip_code> <file_in> <file_out>\n";
        cout << "Valid flip codes: vertical=0, horizontal=1, both=-1\n";
        return -1;
    }
    Mat in_image = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    if (in_image.empty()) { // Check whether the image is read or not
        cout << "Error! Input image cannot be read...\n";
        return -1;
    }
    sscanf(argv[1], "%d", &flip_code); // Read flip code from args.
    flip(in_image, out_image, flip_code); // Caution: Code not checked
    imwrite(argv[3], out_image); // Write the flipped image to file
    namedWindow(win); // Creates a window
    imshow(win, out_image); // Shows output image on window
    cout << "Press any key to exit...\n";
    waitKey(); // Wait infinitely for key press
    return 0;
}