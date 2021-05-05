//
// Created by h_lai on 13/02/19.
//

#ifndef OPENISS_YOLO_DETECTOR_H
#define OPENISS_YOLO_DETECTOR_H

#include "OIDetector.hpp"
#include "OIPythonEnv.hpp"
#include "OIType.hpp"

#include <opencv/cv.hpp>
#include <numpy/ndarrayobject.h>

#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

namespace openiss
{

class OIYoloDetector : public OIDetector
{
public:
    OIYoloDetector();
    ~OIYoloDetector();

    Mat detect_image(Mat &input);
    void detect_bbox(Mat &input, vector<vector<openiss::Point2i>> &bbox, vector<float> &scores);
private:
    PyObject *pInstance, *pResult, *pImageArg;
    PyObject *pFuncDetectBbox, *pFuncDetectImage;
    OIPythonEnv env;

    int init();
};

} // end of namespace

#endif // OPENISS_YOLO_DETECTOR_H
