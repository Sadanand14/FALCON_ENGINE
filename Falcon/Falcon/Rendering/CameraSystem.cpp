#include "CameraSystem.h"
#include <CameraEvent.h>
#include <MouseEvents.h>
#include <KeyEvents.h>

CameraEventSystem* CameraSystem::m_camEvents = nullptr;
CamArray CameraSystem::m_entityCams;
CameraComponent* CameraSystem::m_mainCam = nullptr;
bool CameraSystem::m_cameraMoveable = false;

CameraEventSystem::CameraEventSystem() 
{
	subscribedList.push_back(EVENT_CAMERA_COMPONENT);
	subscribedList.push_back(EVENT_KEY_INPUT);
	subscribedList.push_back(EVENT_MOUSE_INPUT);
	SubscribeToEvents();
}

CameraEventSystem::~CameraEventSystem() 
{
	
}

void CameraEventSystem::SubscribeToEvents() 
{
	for (unsigned int i = 0; i < subscribedList.size(); i++)
	{
		EventManager::SubscribeToEvent(this, subscribedList[i]);
	}
}

void CameraEventSystem::ProcessEvents() 
{
	while (!eventQueue.empty())
	{
		boost::shared_ptr<Event> temp = eventQueue.front();
		eventQueue.pop_front();

		//write response
		if (temp->CheckCategory(EVENT_CAMERA_COMPONENT))
		{
			boost::shared_ptr<CameraEvent> dataEvent = boost::static_pointer_cast<CameraEvent>(temp);
			//if (dataEvent->m_addCamera) 
			CameraSystem::m_entityCams.push_back(static_cast<CameraComponent*>(dataEvent->m_componentPointer));
		}
		else if (temp->CheckCategory(EVENT_MOUSE_INPUT))
		{
			boost::shared_ptr<MouseEvent> mouseEvent = boost::static_pointer_cast<MouseEvent>(temp);
			Mouse_Type type= mouseEvent->GetType();
			if (type == Mouse_Type::CursorEvent) 
			{
				boost::shared_ptr<MouseCursorEvent> cursorEvent = boost::static_pointer_cast<MouseCursorEvent>(mouseEvent);
				CameraSystem::m_mainCam->RotateCamera(cursorEvent->GetOffset());
			}
		}
		else if (temp->CheckCategory(EVENT_KEY_INPUT) && CameraSystem::IsMoveable()) 
		{
			boost::shared_ptr<KeyEvent> keyEvent = boost::static_pointer_cast<KeyEvent>(temp);

			//KeyInputResponse
			if (keyEvent->GetKeyType() == keyType::Released) continue;

			unsigned int key = keyEvent->GetCode();
			switch(key)
			{
			case GLFW_KEY_W:
				CameraSystem::m_mainCam->TranslateCamera(Camera_Move::FORWARD);
				break;
			case GLFW_KEY_S:
				CameraSystem::m_mainCam->TranslateCamera(Camera_Move::BACKWARD);
				break;
			case GLFW_KEY_A:
				CameraSystem::m_mainCam->TranslateCamera(Camera_Move::LEFT);
				break;
			case GLFW_KEY_D:
				CameraSystem::m_mainCam->TranslateCamera(Camera_Move::RIGHT);
				break;
			default:
				return;
			}
		}
	}
}


CameraSystem::CameraSystem() 
{
}

CameraSystem::~CameraSystem() 
{
}

void CameraSystem::Initialize() 
{
	m_camEvents = fmemory::fnew<CameraEventSystem>();
}

void CameraSystem::ShutDown() 
{
	m_entityCams.clear();
	if (m_camEvents != nullptr)
		fmemory::fdelete(m_camEvents);
}
void CameraSystem::Update(float dt) 
{
	m_camEvents->ProcessEvents();

	if (m_mainCam)m_mainCam->Update(dt);
	else 
	{
		m_mainCam = m_entityCams[0];
		m_mainCam->Update(dt);
	}

	CameraType mainType = m_mainCam->GetType();
	if (mainType == CameraType::Fixed || mainType == CameraType::Fixed_Chase)
		m_cameraMoveable = false;
	else
		m_cameraMoveable = true;
}

void CameraSystem::SetMainCam(unsigned int index) 
{
	m_mainCam = m_entityCams[index];
}