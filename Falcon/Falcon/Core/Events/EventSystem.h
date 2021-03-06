#ifndef EVENT_SYSTEM_H_
#define EVENT_SYSTEM_H_

#include <Types.h>
#include "Event.h"
#include <ThreadPool.h>

/**
*Basic EventSystem Definition
*/
class EventSystem
{
protected:
	boost::circular_buffer<boost::shared_ptr<Event>> eventQueue;
	boost::container::vector<EventsCategory> subscribedList;
	ThreadPool* m_threadPool;

public:
	/** Virtual function that processes an event.
	 * Child classes will inherit and figure out how they want to process events
	 */
	virtual void SubscribeToEvents() = 0;
	virtual void ProcessEvents() = 0;
	void ReceiveEvent(boost::shared_ptr<Event> t);
	EventSystem();
};

#endif
