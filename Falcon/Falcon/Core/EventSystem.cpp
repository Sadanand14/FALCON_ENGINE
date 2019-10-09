#include "EventSystem.h"
#include "../System/Log.h"
/** Recieves an event and pushes it to the queue
 * @param (std::shared_ptr<T>) t - The event to push to the queue
 */

EventSystem::EventSystem() 
{
	eventQueue.set_capacity(100);
}	

void EventSystem::ReceiveEvent(boost::shared_ptr<Event> t)
{
	FL_ENGINE_INFO("Pushed an event to event queue");
	FL_ENGINE_INFO("CIRCULAR BUFFER MAXSIZE: {0}", eventQueue.max_size());
	this->eventQueue.push_back(t);
	FL_ENGINE_INFO("event queue size after push {0}", this->eventQueue.size());
}