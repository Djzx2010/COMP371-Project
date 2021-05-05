//
// Created by jash on 04/04/19.
//

#ifndef OPENISS_GESTURE_H
#define OPENISS_GESTURE_H

#include "OIDepthFrame.hpp"
#include "OIGestureType.hpp"
#include "OIGestureTracker.hpp"
#include "OIGestureData.hpp"
#include "OIHandData.hpp"
#include "OIState.hpp"

#ifdef OPENISS_NITE_SUPPORT
    #include "OINiTEGestureTracker.hpp"
    #ifdef OPENISS_ROS_SUPPORT
        #include "OIROSNiTEGestureAdapter.hpp"
    #endif
#endif

#ifdef OPENISS_NUITRACK_SUPPORT
    #include "OINuiTrackGestureTracker.hpp"
    #ifdef OPENISS_ROS_SUPPORT
        #include "OIROSNuiTrackGestureAdapter.hpp"
    #endif
#endif

#endif // OPENISS_GESTURE_H
