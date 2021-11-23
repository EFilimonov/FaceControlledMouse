#include "pch.h"
#include "Timer.h"


   void  Timer::start()
    {
        m_StartTime = std::chrono::system_clock::now();
        m_bRunning = true;
    }

    void Timer::stop()
    {
      //  m_EndTime = std::chrono::system_clock::now();
        m_bRunning = false;
    }

    double Timer::elapsedMilliseconds()
    {
        std::chrono::time_point<std::chrono::system_clock> endTime;

        if (m_bRunning)
        {
            endTime = std::chrono::system_clock::now();
        }
        else
        {
            return 0;
         //   endTime = m_EndTime;
        }

        return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
    }

    double Timer::elapsedSeconds()
    {
        return elapsedMilliseconds() / 1000.0;
    }
