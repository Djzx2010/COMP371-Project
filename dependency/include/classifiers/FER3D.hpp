//
// Created by h_lai on 01/04/19
//

#ifndef OPENISS_FER3D_H
#define OPENISS_FER3D_H

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

class FER3D : public OIPythonClassifier
{
public:
    FER3D();
    ~FER3D();

    cv::Mat predict(std::vector<cv::Mat>& imageVector);

private:
    //PyObject *pInstance, *pResult;
    PyObject *pImageArg1, *pImageArg2, *pImageArg3, *pImageArg4;
    //PyObject *pFuncPredict;
    //OIPythonEnv env;

    int init();
};

}

#endif // OPENISS_FER3D_H