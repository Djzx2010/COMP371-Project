//
// Created by jash on 29/03/19.
//

/*
 * This essentially contains two functions from NiTE2
 * sample utils. Might come in handy so reusing with
 * credits to NiTE2.0
 */

#ifndef OPENISS_UTILS_H
#define OPENISS_UTILS_H

#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>

#include "OIType.hpp"

namespace openiss
{
    // TODO: move to OIType
    // TODO: fix method names
    template<int BufferSize>
    class HistoryBuffer
    {
    public:
        HistoryBuffer() : m_currentHead(-1), m_size(0) {}

        void AddPoint(const Point3f &point) {
            ++m_currentHead;
            if (m_currentHead == BufferSize)
                m_currentHead = 0;
            m_points[m_currentHead] = point;
            if (m_size < BufferSize)
                ++m_size;
        }

        int GetSize() { return m_size; }

        const Point3f &operator[](int index) {
            return m_points[(m_currentHead - index + BufferSize) % BufferSize];
        }

    private:
        friend class Iterator;

        static const int m_bufferSize = BufferSize;
        Point3f m_points[BufferSize];
        int m_currentHead;
        int m_size;
    };

    // TODO: fix method names
    // TODO: ? merge with OITools?
    class FPSCounter
    {
    private:
        uint32_t start_time;
        uint32_t pause_time;
        bool paused;
        bool started;

    public:
        FPSCounter();
        virtual ~FPSCounter();

        void Start();
        void Stop();
        void Pause();
        void Play();
        uint32_t GetTicks();
        bool IsStarted();
        bool IsPaused();
    };
}

#endif // OPENISS_UTILS_H
