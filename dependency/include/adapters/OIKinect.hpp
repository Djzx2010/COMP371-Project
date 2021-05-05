//
// Created by Haotao Lai on 2018-08-01.
//

#ifndef OPENISS_KINECT_H
#define OPENISS_KINECT_H

#include <iostream>
#include <OpenNI.h>

#include "OIDevice.hpp"

namespace openiss
{
class OIFrame;

class OIKinect : public OIDevice
{

public:
    OIKinect();
    ~OIKinect();

    void* rawDevice() override;

    void init() override;
    void open() override;
    void close() override;

    bool enableColor() override;
    bool enableDepth() override;
    bool enableRegistered() override;
    bool enable() override;

    OIFrame* readFrame(StreamType typeFrame) override;
    Intrinsic getIntrinsic(StreamType streamType) override;
    Extrinsic getExtrinsic(StreamType from, StreamType to) override;
    
    float getDepthScale() override;

protected:

    openni::VideoFrameRef mColorFrame;
    openni::VideoFrameRef mDepthFrame;
    openni::VideoFrameRef mIrFrame;

    openni::Device mDevice;
    openni::VideoStream *mpDepthStream;
    openni::VideoStream *mpIrStream;
    openni::VideoStream *mpColorStream;
    openni::Status mStatus;

    openni::VideoStream *getStream(int type);
};

} // end of namespace

#endif // OPENISS_KINECT_H
