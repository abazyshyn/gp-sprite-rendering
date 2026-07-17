#pragma once

namespace GP
{

    class CTimer
    {
    public:
        CTimer();

        bool Init();
        void Frame();

        float GetFrameTime() const { return m_frameTime; };

    private:
        float m_frequency;
        float m_frameTime;
        int64_t m_startTime;
    };

} // namespace GP
