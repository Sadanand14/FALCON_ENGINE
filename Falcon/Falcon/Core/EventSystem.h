#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <boost/circular_buffer.hpp>
#include "Types.h"
#include "Event.h"

class EventSystem
{
protected:
	boost::circular_buffer<std::shared_ptr<Event>> eventQueue;
	
public:
	virtual void processEvent() = 0;
	
	template <typename T>
	void ReceiveEvent(std::shared_ptr<T> t);
};

#endif
