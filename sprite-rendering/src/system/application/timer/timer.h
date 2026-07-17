#pragma once

namespace GP
{

    class CTimer
    {
    public:
        CTimer();

        bool Init();
        void Frame();

        float GetTime() const;

    private:
        float m_frequency;
        float m_frameTime;
        int64_t m_startTime;
    };

} // namespace GP
