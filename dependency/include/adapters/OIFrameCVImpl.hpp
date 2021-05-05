//
// Created by Haotao Lai on 2018-08-09.
//

#ifndef OPENISS_FRAME_OPENCV_IMPL_H
#define OPENISS_FRAME_OPENCV_IMPL_H

#include "OIFrame.hpp"
#include "OISkeleton.hpp"

#include <opencv2/opencv.hpp>

namespace openiss {

class OIFrameCVImpl : public OIFrame {

public:
    explicit OIFrameCVImpl(const cv::Mat& img);

    int getHeight() const override;
    int getWidth()  const override;

    void save(std::string fileName) override;
    void save(std::string path, std::string fileName);

    void show(const char* winName) override;
    void drawSkeleton(OISkeleton* pSkeleton, vector<JointType>& types) override;

    cv::Mat getOpencvMat() override;
    //cv::Mat getCvMat() const;

private:
    cv::Mat mImg;
};

} // end of namespace

#endif // OPENISS_FRAME_OPENCV_IMPL_H
