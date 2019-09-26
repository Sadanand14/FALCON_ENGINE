#include "EventSystem.h"

void EventSystem::ReceiveEvent(std::shared_ptr<Event> t)
{
	this->eventQueue.push_back(t);
}

void EventSystem::processEvent() 
{
}
