//
// Created by SINO on 2016. 4. 23..
//

#include "qrreader.h"

qrReader::qrReader() { }

qrReader::~qrReader() { }

bool qrReader::find(Mat img) {
    int skipRows = 3;
    int stateCount[5] = {0};
    int currentState = 0;
    for (int row = skipRows - 1; row < img.rows; row += skipRows) {
        stateCount[0] = 0;
        stateCount[1] = 0;
        stateCount[2] = 0;
        stateCount[3] = 0;
        stateCount[4] = 0;
        currentState = 0;

        uchar *ptr = img.ptr<uchar>(row);
        for (int col = 0; col < img.cols; col++) {
            if (ptr[col] < 128) {
                // We're at a black pixel
                if ((currentState & 0x1) == 1) {
                    // We were counting white pixels
                    // So change the state now

                    // W->B transition
                    currentState++;
                }

                // Works for boths W->B and B->B
                stateCount[currentState]++;
            } else {
                // We got to a white pixel...
                if ((currentState & 0x1) == 1) {
                    // W->W change
                    stateCount[currentState]++;
                } else {
                    // ...but, we were counting black pixels
                    if (currentState == 4) {
                        // We found the 'white' area AFTER the finder patter
                        // Do processing for it here
                        if (checkRatio(stateCount)) {
                            // This is where we do some more checks
                        } else {
                            currentState = 3;
                            stateCount[0] = stateCount[2];
                            stateCount[1] = stateCount[3];
                            stateCount[2] = stateCount[4];
                            stateCount[3] = 1;
                            stateCount[4] = 0;
                            continue;
                        }
                        currentState = 0;
                        stateCount[0] = 0;
                        stateCount[1] = 0;
                        stateCount[2] = 0;
                        stateCount[3] = 0;
                        stateCount[4] = 0;
                    } else {
                        // We still haven't go 'out' of the finder pattern yet
                        // So increment the state
                        // B->W transition
                        currentState++;
                        stateCount[currentState]++;
                    }
                }
            }
        }
    }
    return false;
}

void qrReader::draw(Mat img) {
    this->img = img;
}

bool qrReader::checkRatio(int stateCount[]) {
    int totalFinderSize = 0;
    for (int i = 0; i < 5; i++) {
        int count = stateCount[i];
        totalFinderSize += count;
        if (count == 0)
            return false;
    }

    if (totalFinderSize < 7)
        return false;

    // Calculate the size of one module
    int moduleSize = ceil(totalFinderSize / 7.0);
    int maxVariance = moduleSize / 2;

    bool retVal = ((abs(moduleSize - (stateCount[0])) < maxVariance) &&
                   (abs(moduleSize - (stateCount[1])) < maxVariance) &&
                   (abs(3 * moduleSize - (stateCount[2])) < 3 * maxVariance) &&
                   (abs(moduleSize - (stateCount[3])) < maxVariance) &&
                   (abs(moduleSize - (stateCount[4])) < maxVariance));

    return retVal;
}