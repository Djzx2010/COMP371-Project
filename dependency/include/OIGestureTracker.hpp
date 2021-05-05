//
// Created by jash on 25/01/19.
//

#ifndef OPENISS_GESTURE_TRACKER_H
#define OPENISS_GESTURE_TRACKER_H

#include <vector>

#include "OITracker.hpp"
#include "OIStatus.hpp"
#include "OIDepthFrame.hpp"
#include "OIGestureData.hpp"
#include "OIHandData.hpp"

namespace openiss
{

class OIGestureTracker : public OITracker
{
public:
    virtual ~OIGestureTracker() = default;

    // OITracker API
    virtual void startTracking() override;
    virtual void stopTracking() override;
    // TODO: fix this up to implement properly in the decendants
    virtual OITrackerFrame* readFrame(OITrackerFrame* p_poTrackerFrame) override;

    //Start/Stop gesture detection and get a collection of gestures available.
    virtual OIStatus startGestureDetection() = 0;
    virtual OIStatus stopGestureDetection() = 0;
    virtual std::vector<OIGestureData> getGestures();

    //Start/Stop hand tracking and get a collection of hands available.
    virtual OIStatus startHandTracking() = 0;
    virtual OIStatus stopHandTracking() = 0;
    virtual std::vector<OIHandData> getHands();

    //Real coordinates to Projective coordinates.
    virtual void convertHandCoordinatesToDepth(float p_x, float p_y, float p_z, float* p_OutX, float* p_OutY) = 0;
    virtual void convertDepthCoordinatesToHand(int p_x, int p_y, int p_z, float* p_OutX, float* p_OutY) = 0;

    //TODO: Needs to be refactored up in the hierarchy.
    virtual OIDepthFrame* getDepthFrame() = 0;

protected:
    OIStatus m_eStatus;

    OIGestureData m_eGesture;
    OIHandData m_eHand;

    std::vector<OIGestureData> m_eGestureData;
    std::vector<OIHandData> m_eHandData;

#ifdef OPENISS_INSTRUMENTATION
    int m_depth_callback_counter;
    int m_gesture_callback_counter;
    int m_hand_callback_counter;
#endif

};

} // namespace

#endif // OPENISS_GESTURE_TRACKER_H
