#ifndef EVENT_H_
#define EVENT_H_

#include <functional>

typedef std::function<void()> EventFunction;

struct Event
{
	EventFunction task;

public:
	virtual ~Event()
	{
	}
};

#endif

