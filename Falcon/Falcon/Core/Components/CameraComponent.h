#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "BasicComponent.h"
#include "TransformComponent.h"
#include <Types.h>
#include <Camera.h>
#include <CameraEvent.h>
#include <EventManager.h>

/**
*Structure Definition for holding data relevant for getting the view and projection matrices from this entity's location.
*/
struct CameraComponent : public BasicComponent
{
private:
	glm::vec3 m_entityPos;
	glm::vec3 m_forward;
	glm::vec3 m_offsetPos;
	glm::vec3 m_offsetRot;
	Transform* m_localTrans;

public:
	
	//CameraComponent() :m_offsetPos({ 0.0f,0.0f,0.0f }), m_offsetRot(0.0f, 0.0f, 0.0f), m_localTrans(nullptr)
	//{}

	CameraComponent(Transform* transform) 
		:m_offsetPos({ 0.0f,0.0f,-10.0f }), m_offsetRot(0.0f, 0.0f, 0.0f), m_localTrans(transform)
	{
		m_entityPos = transform->GetPosition();
		m_forward = glm::vec3(0.0, 0.0, 1.0f) * transform->GetRotation(); 
		EventManager::PushEvent(boost::make_shared <CameraEvent>(this), EVENT_CAMERA_COMPONENT);
	}
	~CameraComponent() {}

	inline glm::vec3 GetPos() { return m_entityPos; }
	inline glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_entityPos + m_offsetPos, (m_entityPos + m_offsetPos) +
			(m_forward + m_offsetRot), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Update() {
		m_entityPos = m_localTrans->GetPosition();
		m_forward = glm::vec3(0.0, 0.0, 1.0f) * m_localTrans->GetRotation();
	}
};
#endif // !1