#ifndef OPENISS_NULL_DEVICE_H
#define OPENISS_NULL_DEVICE_H

#include "OIDevice.hpp"
#include "OIDataFrame.hpp"

namespace openiss
{

#define OPENISS_DEFAULT_NULL_DEVICE_WIDTH 640
#define OPENISS_DEFAULT_NULL_DEVICE_HEIGHT 480

/**
 * Null Device is for build tests and extensions.
 *
 * It's a no-op device for no-dependency builds
 * It should implement all methods and by returning
 * empty or pre-genrated fixtures for what other
 * components needs.
 *
 * Implementations can also extend this device
 * to implement only parts that they may have
 * while leaving out defaul unimplemented APIs.
 *
 * @author Serguei Mokhov
 */
class OINullDevice : public OIDevice
{

public:

    OINullDevice();
    ~OINullDevice();

    void* rawDevice() override;

    void init() override;
    void open() override;
    void close() override;

    bool enableColor() override;
    bool enableDepth() override;
    bool enableRegistered() override;
//    bool enable() override;

    OIFrame* readFrame(StreamType typeFrame) override;
    Intrinsic getIntrinsic(StreamType streamType) override;
    Extrinsic getExtrinsic(StreamType from, StreamType to) override;
    float getDepthScale() override;

protected:

	bool m_bInitialized = false;

	OIDataFrame* m_poGeneratedDepth = nullptr;
	OIDataFrame* m_poGeneratedColor = nullptr;
	OIDataFrame* m_poGeneratedIR = nullptr;
};

}; // namespace

#endif // OPENISS_NULL_DEVICE_H

// EOF
