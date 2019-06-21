//

#include "Timer.h"

//static declarations
TimeDuration Timer::deltaTime;
TimePoint Timer::curr = SteadyClock::now();
TimePoint Timer::prev = Timer::curr;
TimePoint Timer::startTime = Timer::curr;
float Timer::dt;
float Timer::tt = 0;

// this method calculates the dt since the last frame.
void Timer::update()
{
	prev = curr;
	curr = SteadyClock::now();
	deltaTime = curr - prev;
	dt = (float)deltaTime.count();

	tt += dt;
}

float Timer::GetTotalTime()
{
	return tt;
}

float Timer::GetDeltaTime()
{
	return dt;
}

TimeDuration Timer::GetCurrentTime()
{
	using namespace std;
	auto time = SteadyClock::now();
	TimeDuration duration = time - startTime;
	return duration;
}