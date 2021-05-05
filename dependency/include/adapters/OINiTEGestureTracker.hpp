//
// Created by jash on 25/01/19.
//

#ifndef OPENISS_NITE_GESTURE_TRACKER_H
#define OPENISS_NITE_GESTURE_TRACKER_H

#include <NiTE.h>
#include <iostream>
#include "OIGestureTracker.hpp"

namespace openiss
{
    class OINiTEGestureTracker : public OIGestureTracker
    {
    public:
        OINiTEGestureTracker();
        ~OINiTEGestureTracker() override;
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
        std::vector<OIGestureData> getGestures() override;
        std::vector<OIHandData> getHands() override;
        /**/
        OIDepthFrame* getDepthFrame() override;
        /**/
        void convertHandCoordinatesToDepth(float p_x, float p_y, float p_z, float* p_OutX, float* p_OutY) override;
        void convertDepthCoordinatesToHand(int p_x, int p_y, int p_z, float* p_OutX, float* p_OutY) override;

    private:
        /**/
        class FrameListener : public nite::HandTracker::NewFrameListener
        {
        public:
            explicit FrameListener(OINiTEGestureTracker* p_OINiTEGestureTracker)
            {
                this->m_OINiTEGestureTracker = p_OINiTEGestureTracker;
            }
            ~FrameListener()
            {
                delete m_OINiTEGestureTracker;
            }
            OIDepthFrame m_GFrame;
            void onNewFrame(nite::HandTracker &p_HandTracker) override;

        private:
            OINiTEGestureTracker* m_OINiTEGestureTracker;

        };
        /**/
        FrameListener* m_listener;
        nite::Status m_eNiTEStatus;
        nite::HandId m_iHandID;
        nite::HandTracker m_oHandTracker;
        nite::HandTrackerFrameRef m_oHandTrackerFrame;
        nite::Point3f m_GesturePosition;
        openni::VideoFrameRef m_DepthFrame;
    };

} // namespace

#endif // OPENISS_NITE_GESTURE_TRACKER_H
