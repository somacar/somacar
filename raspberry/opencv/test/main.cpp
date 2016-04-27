/*
 * webcam-demo.cpp
 *
 * A demo program of End-to-end Scene Text Detection and Recognition.
 *
 * Created on: Jul 31, 2014
 *     Author: Lluis Gomez i Bigorda <lgomez AT cvc.uab.es>
 */

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"

#include <iostream>


#define REG_CSER 0
#define REG_MSER 1

using namespace std;
using namespace cv;
using namespace cv::text;

//ERStat extraction is done in parallel for different channels
class Parallel_extractCSER : public cv::ParallelLoopBody {
private:
    vector<Mat> &channels;
    vector<vector<ERStat> > &regions;
    vector<Ptr<ERFilter> > er_filter1;
    vector<Ptr<ERFilter> > er_filter2;

public:
    Parallel_extractCSER(vector<Mat> &_channels, vector<vector<ERStat> > &_regions,
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
class Parallel_OCR : public cv::ParallelLoopBody {
private:
    vector<Mat> &detections;
    vector<string> &outputs;
    vector<vector<Rect> > &boxes;
    vector<vector<string> > &words;
    vector<vector<float> > &confidences;
    vector<Ptr<T> > &ocrs;

public:
    Parallel_OCR(vector<Mat> &_detections, vector<string> &_outputs, vector<vector<Rect> > &_boxes,
                 vector<vector<string> > &_words, vector<vector<float> > &_confidences,
                 vector<Ptr<T> > &_ocrs)
            : detections(_detections), outputs(_outputs), boxes(_boxes), words(_words),
              confidences(_confidences), ocrs(_ocrs) { }

    virtual void operator()(const cv::Range &r) const {
        for (int c = r.start; c < r.end; c++) {
            ocrs[c % ocrs.size()]->run(detections[c], outputs[c], &boxes[c], &words[c], &confidences[c], OCR_LEVEL_WORD);
        }
    }

    Parallel_OCR &operator=(const Parallel_OCR &a);
};

class OCRTess {
public:
    void init(int num);

    void set(Mat f);

    void detectAndRecog();

    void show();

    OCRTess(bool downsize, int region, int group);

private:
    //Discard wrongly recognised strings
    bool isRepetitive(const string &s);

    //Draw ER's in an image via floodFill
    void er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat &segmentation);

    int num;
    Mat img, out;
    bool downsize;
    int REGION, GROUP;
    vector<Ptr<OCRTesseract>> ocrs;


    vector<Ptr<ERFilter>> erf1;
    vector<Ptr<ERFilter>> erf2;
};

OCRTess::OCRTess(bool downsize, int region, int group) {
    this->downsize = downsize;
    this->REGION = region;
    this->GROUP = group;

    for (int i = 0; i < 2; i++) {
        Ptr<ERFilter> er_filter1 = createERFilterNM1(loadClassifierNM1("trained_classifierNM1.xml"), 8, 0.00015f, 0.13f, 0.2f, true, 0.1f);
        Ptr<ERFilter> er_filter2 = createERFilterNM2(loadClassifierNM2("trained_classifierNM2.xml"), 0.5);
        this->erf1.push_back(er_filter1);
        this->erf2.push_back(er_filter2);
    }
}

void OCRTess::init(int num) {
    //double t_r = getTickCount();
    this->num = num;
    for (int o = 0; o < num; o++) {
        this->ocrs.push_back(OCRTesseract::create());
    }
    //cout << "TIME_OCR_INITIALIZATION_ALT = "<< ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;
}

void OCRTess::set(Mat f) {
    if (this->downsize)
        resize(f, f, Size(320, 240));
    this->img = f;
}

void OCRTess::detectAndRecog() {
    /*Text Detection*/

    //double t_d = (double)getTickCount();

    Mat grey = Mat::zeros(this->img.rows + 2, this->img.cols + 2, CV_8UC1);
    cvtColor(this->img, grey, CV_BGR2GRAY);

    vector<Mat> channels;
    channels.clear();
    channels.push_back(grey);
    channels.push_back(255 - grey);

    vector<vector<ERStat>> regions(2);
    regions[0].clear();
    regions[1].clear();

    switch (this->REGION) {
        case REG_CSER: {
            parallel_for_(cv::Range(0, (int) channels.size()), Parallel_extractCSER(channels, regions, this->erf1, this->erf2));
            break;
        }
        case REG_MSER: {
            vector<vector<Point> > contours;
            vector<Rect> bboxes;
            Ptr<MSER> mser = MSER::create(21, (int) (0.00002 * grey.cols * grey.rows), (int) (0.05 * grey.cols * grey.rows), 1, 0.7);
            mser->detectRegions(grey, contours, bboxes);
            if (contours.size() > 0)
                MSERsToERStats(grey, contours, regions);
            break;
        }
        default: {
            break;
        }
    }
    //cout << "TIME_REGION_DETECTION_ALT = " << ((double)getTickCount() - t_d)*1000/getTickFrequency() << endl;

    /*Text Recognition (OCR)*/

    // Detect character groups
    //double t_g = getTickCount();
    vector<vector<Vec2i> > nm_region_groups;
    vector<Rect> nm_boxes;
    switch (this->GROUP) {
        case 0: {
            erGrouping(this->img, channels, regions, nm_region_groups, nm_boxes, ERGROUPING_ORIENTATION_HORIZ);
            break;
        }
        case 1: {
            erGrouping(this->img, channels, regions, nm_region_groups, nm_boxes, ERGROUPING_ORIENTATION_ANY, "./trained_classifier_erGrouping.xml", 0.5);
            break;
        }
    }
    //cout << "TIME_GROUPING_ALT = " << ((double)getTickCount() - t_g)*1000/getTickFrequency() << endl;

    this->img.copyTo(this->out);

    int scale = this->downsize ? 2 : 1;
    float scale_img = (float) ((600.f / this->img.rows) / scale);
    float scale_font = (float) (2 - scale_img) / 1.4f;
    vector<string> words_detection;
    float min_confidence1 = 51.f, min_confidence2 = 60.f;

    vector<Mat> detections;

    //t_r = getTickCount();

    for (int i = 0; i < (int) nm_boxes.size(); i++) {
        rectangle(this->out, nm_boxes[i].tl(), nm_boxes[i].br(), Scalar(255, 255, 0), 3);

        Mat group_img = Mat::zeros(this->img.rows + 2, this->img.cols + 2, CV_8UC1);
        er_draw(channels, regions, nm_region_groups[i], group_img);
        group_img(nm_boxes[i]).copyTo(group_img);
        copyMakeBorder(group_img, group_img, 15, 15, 15, 15, BORDER_CONSTANT, Scalar(0));
        detections.push_back(group_img);
    }
    vector<string> outputs((int) detections.size());
    vector<vector<Rect> > boxes((int) detections.size());
    vector<vector<string> > words((int) detections.size());
    vector<vector<float> > confidences((int) detections.size());

    // parallel process detections in batches of ocrs.size() (== num_ocrs)
    for (int i = 0; i < (int) detections.size(); i = i + (int) this->num) {
        Range r;
        if (i + (int) this->num <= (int) detections.size())
            r = Range(i, i + (int) this->num);
        else
            r = Range(i, (int) detections.size());

        parallel_for_(r, Parallel_OCR<OCRTesseract>(detections, outputs, boxes, words, confidences, this->ocrs));
    }


    for (int i = 0; i < (int) detections.size(); i++) {
        outputs[i].erase(remove(outputs[i].begin(), outputs[i].end(), '\n'), outputs[i].end());
        cout << "OCR output = \"" << outputs[i] << "\" lenght = " << outputs[i].size() << endl;
        if (outputs[i].size() < 3)
            continue;

        for (int j = 0; j < (int) boxes[i].size(); j++) {
            boxes[i][j].x += nm_boxes[i].x - 15;
            boxes[i][j].y += nm_boxes[i].y - 15;

//            cout << "  word = " << words[j] << "\t confidence = " << confidences[j] << endl;
            if ((words[i][j].size() < 2) || (confidences[i][j] < min_confidence1) ||
                ((words[i][j].size() == 2) && (words[i][j][0] == words[i][j][1])) ||
                ((words[i][j].size() < 4) && (confidences[i][j] < min_confidence2)) ||
                isRepetitive(words[i][j]))
                continue;
            words_detection.push_back(words[i][j]);
            rectangle(this->out, boxes[i][j].tl(), boxes[i][j].br(), Scalar(255, 0, 255), 3);
            Size word_size = getTextSize(words[i][j], FONT_HERSHEY_SIMPLEX, (double) scale_font, (int) (3 * scale_font), NULL);
            rectangle(this->out, boxes[i][j].tl() - Point(3, word_size.height + 3), boxes[i][j].tl() + Point(word_size.width, 0), Scalar(255, 0, 255), -1);
            putText(this->out, words[i][j], boxes[i][j].tl() - Point(1, 1), FONT_HERSHEY_SIMPLEX, scale_font, Scalar(255, 255, 255), (int) (3 * scale_font));
        }
    }
    //cout << "TIME_OCR_ALT = " << ((double)getTickCount() - t_r)*1000/getTickFrequency() << endl;

    char buff[100];
//    sprintf(buff, "%2.1f Fps. @ %dx%d", (float) (1000 / t_all), out_img.cols, out_img.rows);
    string fps_info = buff;
    rectangle(this->out, Point(this->out.rows - (160 / scale), this->out.rows - (70 / scale)), Point(this->out.cols, this->out.rows), Scalar(255, 255, 255), -1);
    putText(this->out, fps_info, Point(10, this->out.rows - (10 / scale)), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255, 0, 0));
//    putText(this->out, region_types_str[REGION_TYPE], Point(this->out.rows - (150 / scale), this->out.rows - (50 / scale)), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255, 0, 0));
//    putText(this->out, grouping_algorithms_str[GROUPING_ALGORITHM], Point(this->out.rows - (150 / scale), this->out.rows - (30 / scale)), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255, 0, 0));
//    putText(this->out, recognitions_str[RECOGNITION], Point(this->out.rows - (150 / scale), this->out.rows - (10 / scale)), FONT_HERSHEY_DUPLEX, scale_font, Scalar(255, 0, 0));
}

void OCRTess::show() {
    imshow("recognition", this->out);
    //imwrite("recognition_alt.jpg", out_img);
}

bool OCRTess::isRepetitive(const string &s) {
    int count = 0;
    int count2 = 0;
    int count3 = 0;
    int first = (int) s[0];
    int last = (int) s[(int) s.size() - 1];
    for (int i = 0; i < (int) s.size(); i++) {
        if ((s[i] == 'i') ||
            (s[i] == 'l') ||
            (s[i] == 'I'))
            count++;
        if ((int) s[i] == first)
            count2++;
        if ((int) s[i] == last)
            count3++;
    }
    if ((count > ((int) s.size() + 1) / 2) || (count2 == (int) s.size()) || (count3 > ((int) s.size() * 2) / 3)) {
        return true;
    }
    return false;
}

void OCRTess::er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat &segmentation) {
    for (int r = 0; r < (int) group.size(); r++) {
        ERStat er = regions[group[r][0]][group[r][1]];
        if (er.parent != NULL) // deprecate the root region
        {
            int newMaskVal = 255;
            int flags = 4 + (newMaskVal << 8) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
            floodFill(channels[group[r][0]], segmentation, Point(er.pixel % channels[group[r][0]].cols, er.pixel / channels[group[r][0]].cols),
                      Scalar(255), 0, Scalar(er.level), Scalar(0), flags);
        }
    }
}

int main(int argc, char *argv[]) {
    cout << "  Press 'r' to switch between MSER/CSER regions." << endl;
    cout << "  Press 'g' to switch between Horizontal and Arbitrary oriented grouping." << endl;
    cout << "  Press 'o' to switch between OCRTesseract/OCRHMMDecoder recognition." << endl;
    cout << "  Press 's' to scale down frame size to 320x240." << endl;
    cout << "  Press 'ESC' to exit." << endl << endl;

    namedWindow("recognition", WINDOW_NORMAL);
    bool downsize = true;
    int REGION_TYPE = REG_MSER;
    int GROUPING_ALGORITHM = 0;
    int RECOGNITION = 0;
    char *region_types_str[2] = {const_cast<char *>("ERStats"), const_cast<char *>("MSER")};
    char *grouping_algorithms_str[2] = {const_cast<char *>("exhaustive_search"), const_cast<char *>("multioriented")};
    char *recognitions_str[2] = {const_cast<char *>("Tesseract"), const_cast<char *>("NM_chain_features + KNN")};

    OCRTess tess(downsize, REGION_TYPE, GROUPING_ALGORITHM);

    Mat frame;

    //Initialize OCR engine (we initialize 10 instances in order to work several recognitions in parallel)
    cout << "Initializing OCR engines ..." << endl;
    int num_ocrs = 10;
    tess.init(num_ocrs);

    cout << " Done!" << endl;

    VideoCapture cap(0);
    assert(cap.isOpened());

    while (cap.read(frame)) {
        double t_all = (double) getTickCount();

        tess.set(frame);

        tess.detectAndRecog();

        t_all = ((double) getTickCount() - t_all) * 1000 / getTickFrequency();
        char buff[100];
//        sprintf(buff, "%2.1f Fps. @ %dx%d", (float) (1000 / t_all), out_img.cols, out_img.rows);

        tess.show();

        int key = waitKey(30);
        if (key == 27) {
            cout << "esc key pressed" << endl;
            break;
        }
    }

    return 0;
}