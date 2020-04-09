#include "CarSystem.h"
#include <Core/Events/Event.h>
#include <Core/Events/CarEvent.h>


CarEvents::CarEvents()
{
	subscribedList.push_back(EVENT_CAR_CREATED);
	SubscribeToEvents();
}

CarEvents::~CarEvents()
{
}

void CarEvents::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

void CarEvents::ProcessEvents()
{
	while (!eventQueue.empty())
	{
		boost::shared_ptr<Event> temp = eventQueue.front();
		eventQueue.pop_front();

		//write response
		if (temp->CheckCategory(EVENT_CAR_CREATED))
		{
			boost::shared_ptr<CarEvent> dataEvent = boost::static_pointer_cast<CarEvent>(temp);
			CarSystem::AddCar(dataEvent->m_car, dataEvent->m_isUserCar);
			
		}
	}
}	

void CarSystem::AddCar(CarStruct* car, bool isUserCar)
{

	assert(car != nullptr);

	if (isUserCar)
	{
		m_userCars.push_back(car);
	}
	else
	{
		m_AICars.push_back(car);
	}
}
