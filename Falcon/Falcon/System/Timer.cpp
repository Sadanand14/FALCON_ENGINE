//

#include "Timer.h"

//static declarations
TimeDuration Timer::deltaTime;
TimePoint Timer::curr = SteadyClock::now();
TimePoint Timer::prev = Timer::curr;
TimePoint Timer::startTime = Timer::curr;
float Timer::dt;
float Timer::tt = 0;

/**
* Update Class for the timer which will be called every frame to update its data	
*/
void Timer::update()
{
	prev = curr;
	curr = SteadyClock::now();
	deltaTime = curr - prev;
	dt = (float)deltaTime.count();

	tt += dt;
}

/**
* Returns total time since the start of this program as a float value as a summation of all delta times.
*/
float Timer::GetTotalTime()
{
	return tt;
}

/**
* Returns the time difference between the last frame and current frame
*/
float Timer::GetDeltaTime()
{
	return dt;
}

/**
* Returns duration of the program	
*/
TimeDuration Timer::GetCurrentTime()
{
	using namespace std;
	auto time = SteadyClock::now();
	TimeDuration duration = time - startTime;
	return duration;
}