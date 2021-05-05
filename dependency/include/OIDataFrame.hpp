//
// Created by Haotao Lai on 2018-10-24.
//

#ifndef OPENISS_DATAFRAME_H
#define OPENISS_DATAFRAME_H

#include <stdint.h>

#include "OIFrame.hpp"
#include "OIType.hpp"

namespace openiss
{

class OIDataFrame : public OIAbstractDataFrame
{
public:
    explicit OIDataFrame(FrameType type, void* data, int bpp, int width, int height);
    OIDataFrame(OIDataFrame& frame);
    ~OIDataFrame() override;

    void* getData() const override;
    void setData(void*) override;
    int getBytesPerPixel() const override;
    int getHeight() const override;
    int getWidth() const override;

    void save(std::string fileName) override;
    void save(std::string path, std::string fileName) override;
    
    void show(const char* winName) override;
    void drawSkeleton(OISkeleton* pSkeleton, std::vector<JointType>& types) override;

    OIFrame* getFrame();

// TODO: remove coupling
#ifdef OPENISS_OPENCV_SUPPORT
    cv::Mat getOpencvMat() override;
#endif

protected:
    FrameType type;
    int bpp;
    int width, height;
    void* mpData;
    OIFrame* displayCVImpl;

    bool hasOwnData;
    bool hasDispImg;
    void createDisplayImg();
};

}

#endif // OPENISS_DATAFRAME_H
