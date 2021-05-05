//
// Created by Yiran Shen
//

#ifndef OPENISS_FER2D_H
#define OPENISS_FER2D_H

#include "OIPythonClassifier.hpp"
#include "OIPythonEnv.hpp"

#include <opencv/cv.hpp>
#include <numpy/ndarrayobject.h>

#include <vector>
#include <iostream>

using namespace std;
using namespace cv;

namespace openiss
{


class FER2D : public OIPythonClassifier
{
public:
    FER2D();
    ~FER2D();

    cv::Mat predict(cv::Mat& image);

private:

    //PyObject *pInstance, *pResult;
    PyObject *pImageArg1;
    //PyObject *pFuncPredict;
    //OIPythonEnv env;


    int init();
};
}

#endif // OPENISS_FER2D_H
