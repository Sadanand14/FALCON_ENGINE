#include "CameraSystem.h"
#include <CameraEvent.h>

CameraEvents::CameraEvents() 
{
	subscribedList.push_back(EVENT_CAMERA_COMPONENT);
	SubscribeToEvents();
}

CameraEvents::~CameraEvents() 
{
	
}

void CameraEvents::SubscribeToEvents() 
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

void CameraEvents::ProcessEvents() 
{
	while (!eventQueue.empty())
	{
		boost::shared_ptr<Event> temp = eventQueue.front();
		eventQueue.pop_front();

		//write response
		if (temp->CheckCategory(EVENT_CAMERA_COMPONENT))
		{
			boost::shared_ptr<CameraEvent> dataEvent = boost::static_pointer_cast<CameraEvent>(temp);
			m_arr.push_back(static_cast<CameraComponent*>(dataEvent->m_componentPointer));
		}
	}
}


CameraSystem::CameraSystem() 
{
	m_camEvents = fmemory::fnew<CameraEvents>();
}

CameraSystem::~CameraSystem() 
{
	if (m_camEvents != nullptr) fmemory::fdelete(m_camEvents);
}

void CameraSystem::Update() 
{
	m_camEvents->ProcessEvents();
	CamArray* arr = &m_camEvents->m_arr;
	for (unsigned int i = 0; i < arr->size(); ++i) 
	{
		m_entityCams.push_back(arr->at(i));
	}
	arr->clear();

	if (m_mainCam)m_mainCam->Update();
	else 
	{
		m_mainCam = m_entityCams[0];
		m_mainCam->Update();
	}
}

void CameraSystem::SetMainCam(unsigned int index) 
{
	m_mainCam = m_entityCams[index];
}