
#include <chrono>
#include <ctime>
#include <cmath>

class Timer
{
public:
    void start();

    void stop();

    double elapsedMilliseconds();

    double elapsedSeconds();

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
  //  std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};