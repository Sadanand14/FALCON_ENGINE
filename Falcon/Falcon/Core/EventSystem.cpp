#include <Core/EventSystem.h>
#include <System/Log.h>
/** Recieves an event and pushes it to the queue
 * @param (std::shared_ptr<T>) t - The event to push to the queue
 */

EventSystem::EventSystem() 
{
	eventQueue.set_capacity(100);
}	

void EventSystem::ReceiveEvent(boost::shared_ptr<Event> t)
{
	this->eventQueue.push_back(t);
}