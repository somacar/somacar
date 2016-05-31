#ifndef OPENCV_OCR_H
#define OPENCV_OCR_H

#include <iostream>
#include <opencv2/text.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

#define REG_CSER 0
#define REG_MSER 1

#define GR_EX 0
#define GR_MUL 1

#define TR_NM1 string("trained_classifierNM1.xml")
#define TR_NM2 string("trained_classifierNM2.xml")
#define TR_GRP string("trained_classifier_erGrouping.xml")

#define WORD "soma"

using namespace std;
using namespace cv;
using namespace cv::text;

//ERStat extraction is done in parallel for different channels
class Parallel_extractCSER : public ParallelLoopBody {
private:
    vector<UMat> &channels;
    vector<vector<ERStat> > &regions;
    vector<Ptr<ERFilter> > er_filter1;
    vector<Ptr<ERFilter> > er_filter2;

public:
    Parallel_extractCSER(vector<UMat> &_channels, vector<vector<ERStat> > &_regions,
                         vector<Ptr<ERFilter> > _er_filter1, vector<Ptr<ERFilter> > _er_filter2)
            : channels(_channels), regions(_regions), er_filter1(_er_filter1), er_filter2(_er_filter2) { }

    virtual void operator()(const cv::Range &r) const {
        for (int c = r.start; c < r.end; c++) {
            er_filter1[c]->run(channels[c], regions[c]);
            er_filter2[c]->run(channels[c], regions[c]);
        }
    }

    Parallel_extractCSER &operator=(const Parallel_extractCSER &a);
};

//OCR recognition is done in parallel for different detections
template<class T>
class Parallel_OCR : public ParallelLoopBody {
private:
    vector<UMat> &detections;
    vector<string> &outputs;
    vector<vector<Rect> > &boxes;
    vector<vector<string> > &words;
    vector<vector<float> > &confidences;
    vector<Ptr<T> > &ocrs;

public:
    Parallel_OCR(vector<UMat> &_detections, vector<string> &_outputs, vector<vector<Rect> > &_boxes,
                 vector<vector<string> > &_words, vector<vector<float> > &_confidences,
                 vector<Ptr<T> > &_ocrs)
            : detections(_detections), outputs(_outputs), boxes(_boxes), words(_words),
              confidences(_confidences), ocrs(_ocrs) { }

    virtual void operator()(const cv::Range &r) const {
        Mat m;
        for (int c = r.start; c < r.end; c++) {
            m = detections[c].getMat(ACCESS_READ | ACCESS_WRITE);
            ocrs[c % ocrs.size()]->run(m, outputs[c], &boxes[c], &words[c], &confidences[c], OCR_LEVEL_WORD);
        }
    }

    Parallel_OCR &operator=(const Parallel_OCR &a);
};

class OCRTess {
public:
    void init(int num);

    void set(UMat f);

    bool loop();

    bool detectAndRecog();

    void show(bool b);

    OCRTess(bool downsize, int region, int group);

private:
    //Discard wrongly recognised strings
    bool isRepetitive(const string &s);

    //Draw ER's in an image via floodFill
    void er_draw(vector<UMat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, UMat &segmentation);

    int num;
    UMat img, out;
    bool downsize, found;
    int REGION, GROUP;
    vector<Ptr<OCRTesseract>> ocrs;

    vector<Ptr<ERFilter>> erf1;
    vector<Ptr<ERFilter>> erf2;
};

#endif