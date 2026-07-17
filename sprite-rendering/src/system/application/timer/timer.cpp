#include "pch.h"

#include "timer.h"

namespace GP
{

    CTimer::CTimer()
        : m_frequency(0.0f),
          m_frameTime(0.0f),
          m_startTime(0)
    {
    }

    bool CTimer::Init()
    {
        int64_t freaquency{};

        // Get the cycles per second speed for this system.
        QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freaquency));
        if (freaquency == 0)
        {
            return false;
        }

        m_frequency = static_cast<float>(freaquency);

        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_startTime));
        return true;
    }

} // namespace GP
