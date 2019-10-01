#include "EventManager.h"

boost::unordered_map<EventsCategory , std::vector<EventSystem*>> EventManager::systems;

EventManager::EventManager()
{
	
}

EventManager::~EventManager()
{
	
}

/** 
 * Subscribes an event to the system.
 * @param (EventSystem*) sys - The system that needs to subsribe to a certain event
 */

void EventManager::SubscribeToEvent(EventSystem* sys, EventsCategory category) //Const System*?
{
	if (systems.find(category) != systems.end())
		systems[category].push_back(sys);
	else 
	{
		systems[category];
		systems[category].push_back(sys);
	}
}

/**
 * Pushes a new event to the subscribed systems.
 * @param (std::shared_ptr<T>) t - Shared pointer to the event to push
 */
bool EventManager::PushEvent(boost::shared_ptr<Event> event, EventsCategory category)
{
	bool sent = false;
	if(systems.find(category) != systems.end())
	{
		for(int32_t i = 0; i < systems[category].size(); i++)
		{
			systems[category][i]->ReceiveEvent(event);
			sent = true;
		}
	} 
	return sent;
} 
