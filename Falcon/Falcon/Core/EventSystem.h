#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <boost/circular_buffer.hpp>
#include "Event.h"
#include "..//System/ThreadPool.h"

class EventSystem
{
protected:
	boost::circular_buffer<std::shared_ptr<Event>> eventQueue;

	//TODO:Store a list of Events to Subscribe To

public:
	
	/** Virtual function that processes an event.
	 * Child classes will inherit and figure out how they want to process events
	 */
	virtual void processEvents() = 0;
	
	void ReceiveEvent(std::shared_ptr<Event> t);
};

#endif
