#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <boost/circular_buffer.hpp>
#include "Event.h"
#include "..//System/ThreadPool.h"

class EventSystem
{
protected:
	boost::circular_buffer<std::shared_ptr<Event>> eventQueue;
	
public:
	void processEvent();
	
	void ReceiveEvent(std::shared_ptr<Event> t);
};

#endif
