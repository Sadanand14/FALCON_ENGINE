#include "CarSystem.h"
#include <Core/Events/Event.h>
#include <Core/Events/CarEvent.h>
#include <MouseEvents.h>
#include <KeyEvents.h>

CarEventSystem* CarSystem::m_carEvents = nullptr;
CarArr CarSystem::m_userCars;
CarArr CarSystem::m_AICars;
CarStruct* CarSystem::m_currentCar = nullptr;

CarEventSystem::CarEventSystem()
{
	subscribedList.push_back(EVENT_CAR_CREATED);
	subscribedList.push_back(EVENT_KEY_INPUT);
	subscribedList.push_back(EVENT_MOUSE_INPUT);
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

		if (temp->CheckCategory(EVENT_KEY_INPUT))
		{
			//Do the controller logic
			FL_ENGINE_INFO("Received a car input event");
			boost::shared_ptr<KeyEvent> keyEvent = boost::static_pointer_cast<KeyEvent>(temp);

			//KeyInputResponse
			if (keyEvent->GetKeyType() == keyType::Released) continue;

			unsigned int key = keyEvent->GetCode();

			switch (key)
			{
			case GLFW_KEY_UP:
				CarSystem::ApplyInputToUserCar(DriveMode::eDRIVE_MODE_ACCEL_FORWARDS);
				break;
			case GLFW_KEY_DOWN:
				CarSystem::ApplyInputToUserCar(DriveMode::eDRIVE_MODE_ACCEL_REVERSE);
				break;
			case GLFW_KEY_LEFT:
				CarSystem::ApplyInputToUserCar(DriveMode::eDRIVE_MODE_HARD_TURN_LEFT);
				break;
			case GLFW_KEY_RIGHT:
				CarSystem::ApplyInputToUserCar(DriveMode::eDRIVE_MODE_HARD_TURN_RIGHT);
				break;
			case GLFW_KEY_SPACE:
				CarSystem::ApplyInputToUserCar(DriveMode::eDRIVE_MODE_BRAKE);
				break;
			}

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

void CarSystem::ApplyInputToUserCar(DriveMode driveMode)
{
	physics::vehicle::ApplyInputToCar(m_currentCar, driveMode);
}


void CarSystem::AddCar(CarStruct* car, bool isUserCar)
{

	assert(car != nullptr);

	if (isUserCar)
	{
		m_userCars.push_back(car);
		if (m_currentCar == nullptr)
		{
			m_currentCar = m_userCars[0];
		}
	}
	else
	{
		m_AICars.push_back(car);
	}
}
