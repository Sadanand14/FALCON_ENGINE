#include "EventManager.h"

boost::unordered_map<EventManager::EventType, std::vector<System*>> EventManager::systems;

EventManager::EventManager()
{
	
}

EventManager::~EventManager()
{
	
}

/** @brief Registers a new event type
 */
template <typename T>
void EventManager::NewEventType()
{
	static_assert(std::is_base_of<Event, T>::value, "Events must inherit from the Event class");
	//eventTypes.push_back(new T());
	
	systems[TypeToInt::ConvertType<T>()];
}

/** @brief Subscribes an event to the system
 */
template <typename T>
void EventManager::SubscribeToEvent(System* sys) //Const System*?
{
	static_assert(std::is_base_of<Event, T>::value, "Events must inherit from the Event class");
	
	systems[TypeToInt::ConvertType<T>()].push_back(sys);
}

/** @brief Pushes a new event to the subscribed systems
 */
template <typename T>
void EventManager::PushEvent(std::shared_ptr<T> t)
{
	static_assert(std::is_base_of<Event, T>::value, "Events must inherit from the Event class");

	uint64_t mask = TypeToInt::ConvertType<T>();

	if(systems.find(mask) != systems.end())
	{
		for(int32_t i = 0; i < systems[mask].size(); i++)
		{
			systems[mask][i]->ReceiveEvent<T>(t);
		}
	}
} 
