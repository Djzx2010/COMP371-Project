//
// Created by jash on 30/01/19.
//

#ifndef OPENISS_STATE_H
#define OPENISS_STATE_H

// TODO: merge with OIEnum?

namespace openiss
{
    /**/
    enum class OIGestureState
    {
        GESTURE_IS_ABSENT       ,
        GESTURE_IS_COMPLETE     ,
        GESTURE_IS_IN_PROGRESS  ,
    };
    
    /**/
    enum class OIHandState
    {
        HAND_IS_ABSENT          ,
        HAND_IS_NEW             ,
        HAND_IS_LOST            ,
        HAND_IS_TRACKING        ,
        HAND_IS_TOUCHING_FOV    ,
    };
}
#endif // OPENISS_STATE_H
