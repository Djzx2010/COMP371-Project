//
// Created by Yiran Shen on 2018-03-10
//

#ifndef OPENISS_FACE_ADAPTER_H
#define OPENISS_FACE_ADAPTER_H

#include <vector>
#include <memory>

#ifdef OPENISS_OPENCV_SUPPORT
#include <opencv/cv.h>
#endif

#include "OIDevice.hpp"
#include "OIFace.hpp"
#include "OITracker.hpp"

using std::shared_ptr;
using std::vector;

namespace openiss
{

class OIFace;

class OIFaceAdapter // : public OITracker
{
public:
    virtual ~OIFaceAdapter() = default;

#ifdef OPENISS_OPENCV_SUPPORT
    virtual void drawSingleFaceOnImg(cv::Mat& img) = 0;
    virtual void drawMultiFacesOnImg(cv::Mat& img) = 0;
#endif

    virtual void resetAdapterParams() = 0;
    virtual void initializeFace(OIFace& p_oFace) = 0;
};

class OIFaceAdapterFactory
{

public:
    shared_ptr<OIFaceAdapter> createAdapter(const char* adapterName, OIDevice* pDev);
    ~OIFaceAdapterFactory();

private:
    // TODO: rename/refactor; middleware init?
    bool mIsOpenFaceInit = false;
};


} // end of namespace

#endif // OPENISS_FACE_ADAPTER_H
