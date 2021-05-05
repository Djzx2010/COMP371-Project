//
// Created by Haotao Lai on 2018-09-07.
//

#ifndef OPENISS_REALSENSED435_H
#define OPENISS_REALSENSED435_H

#include <opencv2/opencv.hpp>

#include <librealsense2/rs.hpp>

#include "OIDevice.hpp"
#include "OIDataFrame.hpp"

namespace openiss {

class OIRealSenseD435 : public OIDevice {

public:
    OIRealSenseD435();
    ~OIRealSenseD435() override;

    void *rawDevice() override;
    void init() override;
    void open() override;
    void close() override;

    bool enableColor() override;
    bool enableDepth() override;
    bool enableRegistered() override;
    bool enable() override;

    Intrinsic getIntrinsic(StreamType streamType) override;
    Extrinsic getExtrinsic(StreamType from, StreamType to) override;
    float getDepthScale() override;

    OIFrame *readFrame(StreamType streamType) override;

private:

    OIFrame *mpColorOIFrame;
    OIFrame *mpDepthOIFrame;
    OIFrame *mpIrOIFrame;

    rs2::pipeline mPipeline;
    rs2::config mConfig;
    rs2::colorizer mColorMap;

    rs2::pipeline_profile mProfile;
    rs2::stream_profile mDepthProfile;
    rs2::stream_profile mColorProfile;
    rs2::stream_profile mIrProfile;

    rs2::frameset mDataSrc;
    rs2::frame mDepthFrame;
    rs2::frame mColorFrame;
    rs2::frame mIrFrame;

    cv::Size mFrameSize = cv::Size(640, 480);
    float depthScale = 0;
    bool hasDepthScale = false;
};

} // end of namespace

#endif // OPENISS_REALSENSED435_H
