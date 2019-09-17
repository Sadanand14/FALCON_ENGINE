#include "System.h"

template <typename T>
void System::ReceiveEvent(std::shared_ptr<T> t)
{
	this->eventQueue.push_back(t);
}
