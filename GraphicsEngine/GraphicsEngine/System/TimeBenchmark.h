#ifndef TIME_BENCHMARK_H
#define TIME_BENCHMARK_H
#include <chrono>
#include <boost/chrono/chrono.hpp>
#include <iostream>
#include "Log.h"

struct timer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
	std::chrono::duration<float> duration;
	const char* jobTitle;

	timer(const char* job) 
	{
		start = std::chrono::high_resolution_clock::now();
		jobTitle = job;
	}

	~timer() 
	{
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count()*1000.0f;
		//std::cout << jobTitle << " took " << ms << "\n";
		FL_ENGINE_INFO("JOBSYSTEM INFO:{0} took {1}."jobTitle, ms);
	}
};

struct boostTimer 
{
	boost::chrono::time_point<boost::chrono::high_resolution_clock> start, end;
	boost::chrono::duration<float> duration;
	const char* jobTitle;


	boostTimer(const char* job)
	{
		start = boost::chrono::high_resolution_clock::now();
		jobTitle = job;
	}

	~boostTimer()
	{
		end = boost::chrono::high_resolution_clock::now();
		duration = end - start;

		float ms = duration.count()* 1000.0f;
		FL_ENGINE_INFO("JOBSYSTEM INFO:{0} took {1}."jobTitle, ms);
	}
};

#endif // !1
