//
// Created by jash on 15/04/19.
//

#ifndef OPENISS_GESTURE_DATA_H
#define OPENISS_GESTURE_DATA_H

#include "OIState.hpp"
#include "OIDepthFrame.hpp"
#include "OIGestureType.hpp"

namespace openiss
{
/* class OIGestureData represents a real-world gesture with relevant gesture data. */
    class OIGestureData
    {
    public:
        OIGestureData();
        ~OIGestureData();
        /**/
        const Point3f &getGesturePosition();
        const OIGestureType &getGestureType();
        /**/
        bool isGestureComplete() const;
        void setGesturePosition(float p_fx, float p_fy, float p_fz);
        void setGestureType(OIGestureType p_eGestureType);
        void setGestureState(OIGestureState p_eGestureState);
    /**/
    private:
        OIGestureType m_eGestureType;
        Point3f m_sGesturePosition;
        OIGestureState m_iGestureState;
    };
}
#endif // OPENISS_GESTURE_DATA_H
