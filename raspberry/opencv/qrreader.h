//
// Created by SINO on 2016. 4. 23..
//

#ifndef OPENCV_QRREADER_H
#define OPENCV_QRREADER_H


class qrReader {
public:
    qrReader();
    ~qrReader();
    bool find(Mat img);
    void draw(Mat img);

private:
    Mat img;
};


#endif //OPENCV_QRREADER_H
