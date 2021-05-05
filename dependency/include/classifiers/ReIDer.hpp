//
// Created by h_lai on 28/03/19.
//

#ifndef OPENISS_REIDER_H
#define OPENISS_REIDER_H

#include "OIPythonClassifier.hpp"
#include "OIPythonEnv.hpp"
#include "OIType.hpp"

#include <opencv/cv.hpp>
#include <numpy/ndarrayobject.h>

#include <vector>
#include <cstring>
#include <iostream>

using namespace std;
using namespace cv;

namespace openiss
{

class ReIDer : public OIPythonClassifier
{
public:
    ReIDer();
    ~ReIDer();

    string predict(Mat &image1);

private:
    //PyObject *pInstance, *pResult;
    PyObject *pImageArg1;
    //PyObject *pFuncPredict;
    //OIPythonEnv env;

    int init();
};
}

#endif //OPENISS_ReIDer
