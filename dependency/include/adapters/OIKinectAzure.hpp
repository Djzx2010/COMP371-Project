#ifndef OPENISS_KINECT_AZURE_H
#define OPENISS_KINECT_AZURE_H

#include <k4a/k4a.hpp>

#include "OIDevice.hpp"
#include "OIDataFrame.hpp"

namespace openiss
{

class OIKinectAzure : public OIDevice
{

public:
    OIKinectAzure();
    ~OIKinectAzure() override;

    void* rawDevice() override;
    void init() override;
    void open() override;
    void close() override;

    bool enableColor() override;
    bool enableDepth() override;
    bool enableRegistered() override;

    Intrinsic getIntrinsic(StreamType streamType) override;
    Extrinsic getExtrinsic(StreamType from, StreamType to) override;
    float getDepthScale() override;

    OIFrame* readFrame(StreamType streamType) override;

protected:

    const uint32_t m_iDeviceCount = 0;
    k4a_device_configuration_t m_oK4AConfig;
    k4a::device m_oRawDevice;

	bool m_bInitialized;

    //cv::Size mFrameSize = cv::Size(640, 480);
    float depthScale = 0;
    bool hasDepthScale = false;
};

} // end of namespace

#endif // OPENISS_KINECT_AZURE_H
