#ifndef OPENISS_NULL_GESTURE_TRACKER_H
#define OPENISS_NULL_GESTURE_TRACKER_H

#include <iostream>
#include "OIGestureTracker.hpp"

namespace openiss
{
    /**
     * For camera-less development and testing.
     * @author Serguei Mokhov
     */
    class OINullGestureTracker : public OIGestureTracker
    {
    public:
        OINullGestureTracker();
        ~OINullGestureTracker() override;

        /**/
        OIStatus init() override;
        OIStatus update() override;
        OIStatus stop() override;
        
        /**/
        OIStatus startGestureDetection() override;
        OIStatus stopGestureDetection() override;
        
        /**/
        OIStatus startHandTracking() override;
        OIStatus stopHandTracking() override;
        
        /**/
        OIDepthFrame* getDepthFrame() override;

        /**/
        void convertHandCoordinatesToDepth(float p_x, float p_y, float p_z, float* p_OutX, float* p_OutY) override;
        void convertDepthCoordinatesToHand(int p_x, int p_y, int p_z, float* p_OutX, float* p_OutY) override;

    private:

       bool m_bInitialized;
       bool m_bGestureDetectionRunning;
       bool m_bHandTrackingRunning;

       // TMP, remove after integraion.
       OIDepthFrame* m_poDepthFrame;
    };
}
#endif // OPENISS_GESTURE_TRACKER_H
