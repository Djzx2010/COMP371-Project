//
// Created by jash on 20/01/19.
//

#ifndef OPENISS_NUITRACK_GESTURE_TRACKER_H
#define OPENISS_NUITRACK_GESTURE_TRACKER_H

#include <iostream>
#include <vector>
#include <nuitrack/Nuitrack.h>
#include "OIGestureTracker.hpp"

namespace openiss
{
    class OINuiTrackGestureTracker : public OIGestureTracker
    {
    public:
        OINuiTrackGestureTracker();
        ~OINuiTrackGestureTracker() override;
        /**/
        OIStatus init() override;
        OIStatus update() override;
        OIStatus stop() override;
        /**/
        OIStatus startGestureDetection() override;
        OIStatus stopGestureDetection() override;
        /**/
        std::vector<OIHandData> getHands() override;
        std::vector<OIGestureData> getGestures() override;
        /**/
        OIStatus startHandTracking() override;
        OIStatus stopHandTracking() override;
        /**/
        OIDepthFrame* getDepthFrame() override;
        void convertHandCoordinatesToDepth(float p_x, float p_y, float p_z, float* p_OutX, float* p_OutY) override;
        void convertDepthCoordinatesToHand(int p_x, int p_y, int p_z, float* p_OutX, float* p_OutY) override;

    private:
        /**/
        tdv::nuitrack::DepthSensor::Ptr m_DepthSensor;
        tdv::nuitrack::OutputMode m_outputMode;
        tdv::nuitrack::GestureRecognizer::Ptr m_gestureRecognizer;
        tdv::nuitrack::HandTracker::Ptr m_HandTracker;
        tdv::nuitrack::UserTracker::Ptr m_UserTracker;
        tdv::nuitrack::Vector3 m_projectiveCoordinates;
        tdv::nuitrack::Vector3 m_realCoordinates;
        /**/
        void onNewDepthFrame(tdv::nuitrack::DepthFrame::Ptr p_Frame);
        void onNewGestures( tdv::nuitrack::GestureData::Ptr p_gestureData);
        void onHandUpdate( tdv::nuitrack::HandTrackerData::Ptr p_handData );
        void onUserUpdate( tdv::nuitrack::UserFrame::Ptr p_userFrameData );
        void onUserStateChange ( tdv::nuitrack::UserStateData::Ptr p_userStateData);
        void onNewUser ( int p_userID);
        void onLostUser ( int p_userID);
        /**/
        int m_handID = 0;
        OIDepthFrame m_GFrame;
    };

} // namespace

#endif // OPENISS_NUITRACK_GESTURE_TRACKER_H
