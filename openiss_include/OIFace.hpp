//
//  OIFace.hpp
//  Face
//
//  Created by Yiran Shen.
//  Created by Jashanjot Singh on 2018-07-22.

#ifndef OPENISS_FACE_H
#define OPENISS_FACE_H

// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
#include <opencv2/opencv.hpp>
#endif

#include "OIFaceAdapter.hpp"

namespace openiss
{

class OIFace
{
public:
// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
    explicit OIFace(const cv::Mat& img);
#endif

    explicit OIFace();

    ~OIFace();

    //Copy constructor
    OIFace(const OIFace& copied);

    //operator overloads =
    OIFace& operator= (const OIFace& m);

    
    // void getExpressions(openiss::OIFaceAdapter& faceAdapter);
    // void getFaces(openiss::OIFaceAdapter& faceAdapter);
    std::vector<std::vector<openiss::Point2f>>* facialLandmarks;
    
    //Gives a pointer to an empty FacialLandmark type structure
    static std::vector<std::vector<openiss::Point2f>>* getFacialLandmarks();


// TODO: remove coupling on OpenCV
#ifdef OPENISS_OPENCV_SUPPORT
    cv::Mat& getOriginalImg();

// private:
    cv::Mat originalImg;
    std::vector<cv::Mat> faceVector;
    std::vector<std::vector<cv::Point2f>> facialLandmarksVector;
    std::vector<cv::Mat> expressionVector;
#endif

};

} // namespace openiss

#endif /* OPENISS_FACE_H */
