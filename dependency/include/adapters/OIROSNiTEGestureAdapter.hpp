//
// Created by jash on 29/04/19.
//

#ifndef OPENISS_ROS_NITE_GESTURE_ADAPTER_H
#define OPENISS_ROS_NITE_GESTURE_ADAPTER_H

#include "ros/ros.h"
#include "OIGestureTracker.hpp"
#include "openiss_gesture_tracker_msgs/DepthFrame.h"
#include "openiss_gesture_tracker_msgs/Gesture.h"
#include "openiss_gesture_tracker_msgs/GestureArray.h"
#include "openiss_gesture_tracker_msgs/Hand.h"
#include "openiss_gesture_tracker_msgs/HandArray.h"

namespace openiss
{

    class OIROSNiTEGestureAdapter : public OIGestureTracker
    {
    public:
        ~OIROSNiTEGestureAdapter() override = default;

        OIStatus init() override;
        OIStatus update() override;
        OIStatus stop() override;

        OIStatus startGestureDetection() override { return OIStatus::STATUS_OK; };
        OIStatus stopGestureDetection() override { return OIStatus::STATUS_OK; };
        std::vector<OIGestureData> getGestures() override;

        OIStatus startHandTracking() override { return OIStatus::STATUS_OK; };
        OIStatus stopHandTracking() override { return OIStatus::STATUS_OK; };
        std::vector<OIHandData> getHands() override;

        void convertHandCoordinatesToDepth(float p_x, float p_y, float p_z, float *p_OutX, float *p_OutY) override;
        void convertDepthCoordinatesToHand(int p_x, int p_y, int p_z, float *p_OutX, float *p_OutY) override;

        OIDepthFrame *getDepthFrame() override;

    private:
        ros::Subscriber m_DepthFrameSubscriber;
        ros::Subscriber m_GesturesSubscriber;
        ros::Subscriber m_HandsSubscriber;
        ros::Subscriber m_HandSubscriber;
        void adaptDepthData( const openiss_gesture_tracker_msgs::DepthFrame& p_depthFrameMsg );
        void adaptGestureData( const openiss_gesture_tracker_msgs::GestureArray& p_GesturesMsg );
        void adaptHandData( const openiss_gesture_tracker_msgs::HandArray& p_HandsMsg );

        /**/
        OIDepthFrame m_DepthFrame;
        const uint16_t* m_depthData = nullptr;
        // subscriber queue size, ideally 0 (infinity)
        int m_msg_queue_size;
    };
    
} // namespace

#endif // OPENISS_ROS_NITE_GESTURE_ADAPTER_H
