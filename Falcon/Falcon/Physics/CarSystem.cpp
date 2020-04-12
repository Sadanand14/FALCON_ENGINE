#include "CarSystem.h"
#include <Core/Events/Event.h>
#include <Core/Events/CarEvent.h>

CarEventSystem* CarSystem::m_carEvents = nullptr;
CarArr CarSystem::m_userCars;
CarArr CarSystem::m_AICars;


CarEventSystem::CarEventSystem()
{
	subscribedList.push_back(EVENT_CAR_CREATED);
	SubscribeToEvents();
}

CarEventSystem::~CarEventSystem()
{
}

void CarEventSystem::SubscribeToEvents()
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

void CarEventSystem::ProcessEvents()
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


void CarSystem::Initialize()
{
	m_carEvents = fmemory::fnew<CarEventSystem>();
}

void CarSystem::ShutDown()
{
	fmemory::fdelete<CarEventSystem>(m_carEvents);
	m_userCars.clear();
	m_AICars.clear();
}

void CarSystem::Update()
{
	m_carEvents->ProcessEvents();
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
