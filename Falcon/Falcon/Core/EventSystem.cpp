#include "EventSystem.h"

template <typename T>
void EventSystem::ReceiveEvent(std::shared_ptr<T> t)
{
	this->eventQueue.push_back(t);
}
