#ifndef CAMERA_EVENT_H
#define CAMERA_EVENT_H

#include <Event.h>
#include <Components/BasicComponent.h>

struct CameraEvent : public Event 
{
	BasicComponent * m_componentPointer = nullptr;
	CameraEvent(BasicComponent* cam): Event(EVENT_CAMERA_COMPONENT),m_componentPointer(cam)
	{}
};

#endif//