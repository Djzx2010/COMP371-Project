//
// Created by jash on 25/01/19.
//

#ifndef OPENISS_GESTURE_TYPE_H
#define OPENISS_GESTURE_TYPE_H

namespace openiss
{
    enum OIGestureType
    {
        /* Default Initialized */
        GESTURE_ALL,

        /* NiTE2 Gesture Types */
        GESTURE_WAVE,
        GESTURE_CLICK,
        GESTURE_HAND_RAISE,

        /* Nuitrack Gesture Types */
        GESTURE_SWIPE_LEFT,
        GESTURE_SWIPE_RIGHT,
        GESTURE_SWIPE_UP, // TODO: same as GESTURE_HAND_RAISE?
        GESTURE_SWIPE_DOWN,
        GESTURE_PUSH,
        GESTURE_WAVING = GESTURE_WAVE
    };
}

#endif // OPENISS_GESTURE_TYPE_H
