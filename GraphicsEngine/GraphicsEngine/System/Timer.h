#ifndef TIMER_H_
#define TIMER_H_
#include <chrono>

typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::duration<float> TimeDuration;
typedef std::chrono::steady_clock SteadyClock;

class Timer
{

private:
	static TimePoint curr, prev, startTime;
	static TimeDuration deltaTime;
	static float dt, tt;

public:
	static float GetTotalTime();
	static float GetDeltaTime();
	static void update();
	static TimeDuration GetCurrentTime();
};

#endif // TIMER_H_