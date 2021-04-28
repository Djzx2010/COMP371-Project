//
//  OIFace.cpp
//  Face
//  
// OIFace is based on Opencv
//  Created by Yiran Shen on 2018-07-22.
//

#include "OIFace.hpp"

// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
openiss::OIFace::OIFace(const cv::Mat& img)
{
    originalImg = img;
}
#endif

openiss::OIFace::OIFace()
{
    this->facialLandmarks = new std::vector<std::vector<openiss::Point2f>>;
}

openiss::OIFace::~OIFace()
{
    delete facialLandmarks;
}

openiss::OIFace::OIFace(const OIFace& copied)
{
    this->facialLandmarks = new std::vector<std::vector<openiss::Point2f>>;
    this->facialLandmarks = copied.facialLandmarks;
}

openiss::OIFace& openiss::OIFace::operator=(const OIFace& oif)
{
    this->facialLandmarks = oif.facialLandmarks;
    return *this;
}

std::vector<std::vector<openiss::Point2f>>* openiss::OIFace::getFacialLandmarks()
{
    std::vector<std::vector<openiss::Point2f>>* FacialLandmarks = new std::vector<std::vector<openiss::Point2f>>;
    return FacialLandmarks;
}

// std::vector<std::vector<cv::Point2f>>* openiss::OIFace::getFacialLandmarks(openiss::OIFaceAdapter& faceAdapter) {

// }

// void openiss::OIFace::getExpressions(openiss::OIFaceAdapter& faceAdapter) {

// }

// void openiss::OIFace::getFaces(openiss::OIFaceAdapter& faceAdapter) {

// }

#ifdef OPENISS_OPENCV_SUPPORT
cv::Mat& openiss::OIFace::getOriginalImg()
{
    return this->originalImg;
}
#endif
