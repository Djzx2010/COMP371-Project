#ifndef OPENISS_OISTRUCTURE
#define OPENISS_OISTRUCTURE

#include <opencv2/opencv.hpp>

#include <ST/CaptureSession.h>

#include "OIDevice.hpp"
#include "OIDataFrame.hpp"

namespace openiss {

struct SessionDelegate : ST::CaptureSessionDelegate {
    ST::DepthFrame mDepthFrame;
    ST::ColorFrame mColorFrame;
    ST::InfraredFrame mInfraredFrame;
    bool receivedFrame = false;
    bool isStreaming = false;

    void captureSessionEventDidOccur(ST::CaptureSession *session, ST::CaptureSessionEventId event) override {
        printf("Received capture session event %d (%s)\n", (int)event, ST::CaptureSessionSample::toString(event));
        switch (event) {
            case ST::CaptureSessionEventId::Booting: break;
            case ST::CaptureSessionEventId::Connected:
                printf("Starting streams...\n");
                printf("Sensor Serial Number is %s \n ", session->sensorInfo().serialNumber);
                session->startStreaming();
                break;
            case ST::CaptureSessionEventId::Disconnected:
                session->stopStreaming();
                break;
            case ST::CaptureSessionEventId::Error:
                printf("Capture session error\n");
                exit(1);
                break;
            default:
                printf("Capture session event unhandled\n");
        }
    }

    void captureSessionDidOutputSample(ST::CaptureSession *, const ST::CaptureSessionSample& sample) override {
        receivedFrame = false;
        switch (sample.type) {
            case ST::CaptureSessionSample::Type::DepthFrame:
                receivedFrame = true;
                mDepthFrame = sample.depthFrame;
                break;
            case ST::CaptureSessionSample::Type::VisibleFrame:
                receivedFrame = true;
                mColorFrame = sample.visibleFrame;
                break;
            case ST::CaptureSessionSample::Type::InfraredFrame:
                receivedFrame = true;
                mInfraredFrame = sample.infraredFrame;
                break;
            case ST::CaptureSessionSample::Type::SynchronizedFrames:
                receivedFrame = true;
                mColorFrame = sample.visibleFrame;
                mDepthFrame = sample.depthFrame;
                mInfraredFrame = sample.infraredFrame;
                break;
            default:
                printf("Sample type unhandled\n");
        }
        if(receivedFrame == true && !isStreaming){
            isStreaming = true;
        }
    }
};

class OIStructure : public OIDevice {

public:
    OIStructure();
    ~OIStructure() override;

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

    ST::CaptureSession session;
    ST::CaptureSessionSettings settings;

    SessionDelegate delegate;

    bool streamStarted;
    double lastColorFrameTimestamp = 0.0;
    double lastDepthFrameTimestamp = 0.0;
    double lastInfraredFrameTimestamp = 0.0;
};

} // end of namespace

#endif //OPENISS_OISTRUCTURE
