#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <type_traits>
#include <memory>
#include <boost/unordered_map.hpp>
#include <boost/container/vector.hpp>
#include "Event.h"
#include "System.h"
#include "Types.h"
#include "TypeToInt.h"

class EventManager
{
private:
	typedef u32 EventType;
	static boost::unordered_map<EventType, std::vector<System*>> systems;
	
public:
	EventManager();
	~EventManager();
	
	template <typename T>
	static void NewEventType();
	
	template <typename T>
	static void SubscribeToEvent(System* sys);
	
	template <typename T>
	static void PushEvent(std::shared_ptr<T> t);
};

#endif 
