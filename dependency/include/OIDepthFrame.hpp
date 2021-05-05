//
// Created by jash on 29/03/19.
//

#ifndef OPENISS_DEPTH_FRAME_H
#define OPENISS_DEPTH_FRAME_H

#include <iostream>
#include <cstring>
#include <stdint.h>

#include "OIDataFrame.hpp"

namespace openiss
{

/**/
    // TODO: refactor away
    struct OIDepthFrame : public OIDataFrame
    {
    public:
        OIDepthFrame()
            : OIDataFrame(FrameType::DEPTH_FRAME, nullptr, 2, 0, 0) { };

        ~OIDepthFrame() = default;

        /**/
        void setHeight(const int& p_height)
        {
            this->m_height = p_height;
        }
        void setWidth(const int& p_width)
        {
            this->m_width = p_width;
        }
        void setDepthData( const uint16_t* p_depthData)
        {
            this->m_DepthData = p_depthData;
        }
        /**/
        const uint16_t* getDepthData()
        {
            return this->m_DepthData;
        }
        int getHeight()
        {
            return this->m_height;
        }
        int getWidth()
        {
            return this->m_width;
        }
    /**/
    private:
        int m_height {0};
        int m_width {0};
        const uint16_t* m_DepthData{nullptr};
    };

}

#endif // OPENISS_DEPTH_FRAME_H
