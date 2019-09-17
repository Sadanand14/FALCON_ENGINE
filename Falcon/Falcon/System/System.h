#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <boost/circular_buffer.hpp>
#include "Types.h"
#include "Event.h"

class System
{
protected:
	boost::circular_buffer<std::shared_ptr<Event>> eventQueue;
	
public:
	virtual void processEvent() = 0;
	
	template <typename T>
	void ReceiveEvent(std::shared_ptr<T> t);
};

#endif
