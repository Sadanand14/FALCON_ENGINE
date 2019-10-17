#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <type_traits>
#include <memory>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/container/vector.hpp>
#include <Core/Event.h>
#include <Core/EventSystem.h>
#include <System/Types.h>

/**
*Manages events being pushed and what systems are listening to which events.
*/

class EventManager
{
private:
	static boost::unordered_map<EventsCategory, std::vector<EventSystem*>> systems;
	
public:
	EventManager();
	~EventManager();
	
	static void SubscribeToEvent(EventSystem* sys, EventsCategory category);
	
	static bool PushEvent(boost::shared_ptr<Event> event, EventsCategory category);
};

#endif 
