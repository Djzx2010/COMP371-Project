//
// Created by Haotao Lai on 2018-08-08.
//

#ifndef OPENISS_DEVICE_H
#define OPENISS_DEVICE_H

#include <vector>
#include <memory>

#include "OIFrame.hpp"
#include "OIEnum.hpp"
#include "OIType.hpp"
#include "OIStatus.hpp"

namespace openiss
{

/**
 * Abstraction of actual ddevice (e.g., depth cameras, web cams, or software devices).
 * @author Haotao Lai
 * @author Serguei Mokhov
 */
class OIDevice
{
public:

    OIDevice();
    virtual ~OIDevice();

    virtual void* rawDevice() = 0;

    // TODO: should be boolean or OIStatus if init successful
    virtual void init() = 0;

    virtual void open() = 0;
    virtual void close() = 0;

    /**
     * Enumerate connected devices of the same type.
     */
    //virtual std::vector <std::shared_ptr<OIDevice>> enumerateDevices() const;

    virtual bool enableColor();
    virtual bool enableDepth();
    virtual bool enableRegistered();
    virtual bool enable();

    virtual OIStatus getStatus();

    virtual Intrinsic getIntrinsic(StreamType streamType) = 0;
    virtual Extrinsic getExtrinsic(StreamType from, StreamType to) = 0;
    virtual float getDepthScale() = 0;

    // need to manage the heap-allocated memory of the frame
    virtual OIFrame* readFrame(StreamType frameType) = 0;

protected:

    OIStatus m_eStatus;

    bool m_is_enable_rgb = false;
    bool m_is_enable_depth = false;
    bool m_is_enable_registered = false;

    // TODO: Ending should be Frame, not OIFrame
    OIFrame* mpColorOIFrame;
    OIFrame* mpDepthOIFrame;
    OIFrame* mpIrOIFrame;

};

/**
 * This class provides different kind of device
 * object based on the passing paramenter of the
 * create() method.
 * 
 * Also, it will responsible for managing the memory 
 * of those device objects it created.
 */
class OIDeviceFactory
{
public:
    std::shared_ptr<OIDevice> create(const char* devName);
    //std::vector <std::shared_ptr<OIDevice>> enumerateDevices(const char* devName) const;

private:

// TODO: Should not be here
#ifdef OPENISS_OPENNI2_SUPPORT
    bool mIsOpenNIInit = false;
#endif
};

} // end of namespace

#endif // OPENISS_DEVICE_H
