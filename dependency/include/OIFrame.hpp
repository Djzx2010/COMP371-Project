//
// Created by Haotao Lai on 2018-08-09.
//

#ifndef OPENISS_FRAME_H
#define OPENISS_FRAME_H

#include <vector>
#include <string>

#include "OIEnum.hpp"

// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
#include <opencv2/opencv.hpp>
#endif

namespace openiss
{

class OISkeleton;

class OIFrame
{
public:
    virtual ~OIFrame() = default;

    virtual int getHeight() const = 0;
    virtual int getWidth() const = 0;

// TODO: these 4 do not belong to Frame
    virtual void save(std::string fileName) = 0;
    virtual void save(std::string path, std::string fileName) = 0;
    virtual void show(const char* winName) = 0;
    virtual void drawSkeleton(OISkeleton* pSkeleton, std::vector<openiss::JointType>& types) = 0;

// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
    virtual cv::Mat getOpencvMat() = 0;
#endif
};

class OIAbstractDataFrame : public OIFrame
{
public:
    ~OIAbstractDataFrame() override = default;
    virtual void* getData() const = 0;
    virtual void setData(void*) = 0;
    virtual int getBytesPerPixel() const = 0;
};

} // end of namespace
#endif // OPENISS_FRAME_H
