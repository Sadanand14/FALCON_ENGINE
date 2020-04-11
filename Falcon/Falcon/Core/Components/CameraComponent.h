#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "BasicComponent.h"
#include "TransformComponent.h"
#include <Types.h>
#include <CameraEvent.h>
#include <EventManager.h>
#include <cmath>

enum class Camera_Move { FORWARD, BACKWARD, LEFT, RIGHT };
enum class CameraType {Free, Fixed,Fixed_Chase, Free_Chase};

/**
*Structure Definition for holding data relevant for getting the view from this entity's location.
*/
struct CameraComponent : public BasicComponent
{
private:
	CameraType m_type;
	glm::vec3 m_currentPos, m_forward , m_offsetPos, m_offsetRot, m_right, m_up, m_entityPos, m_entityRot;
	float m_yaw, m_pitch;
	Transform* m_localTrans;
	float timestep;

public:
	
	//CameraComponent() :m_offsetPos({ 0.0f,0.0f,0.0f }), m_offsetRot(0.0f, 0.0f, 0.0f), m_localTrans(nullptr)
	//{}

	inline CameraType GetType() { return m_type; }

	CameraComponent(Transform* transform) 
		:m_offsetPos({ 0.0f,0.0f,-10.0f }), m_offsetRot(0.0f, 0.0f, 0.0f), m_localTrans(transform), m_type(CameraType::Free_Chase), m_yaw(-90.0f),
		m_pitch(0.0f)
	{
		m_currentPos = transform->GetPosition();
		m_forward = glm::normalize(glm::vec3(0.0, 0.0, 1.0f) * transform->GetRotation()); 
		m_entityPos = m_currentPos;
		m_entityRot = m_forward;
		m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		m_pitch = asin(-m_forward.y);
		m_yaw = atan2(m_forward.x, m_forward.z);
		EventManager::PushEvent(boost::make_shared <CameraEvent>(this), EVENT_CAMERA_COMPONENT);
	}
	~CameraComponent() {}

	inline glm::vec3 GetPos() { return m_currentPos; }
	inline glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_currentPos + m_offsetPos, (m_currentPos + m_offsetPos) +
			(m_forward + m_offsetRot), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	inline void SetCamera(CameraType type) { m_type = type; }

	void Update(float dt) 
	{
		timestep = dt;
		switch (m_type)
		{
		case CameraType::Fixed: 
			Fixed_Update();
			break;
		case CameraType::Free:
			Free_Update();
			break;
		case CameraType::Fixed_Chase:
			Fixed_Chase_Update();
			break;
		case CameraType::Free_Chase:
			Free_Chase_Update();
			break;
		default:
			return;
		}
	}

	void TranslateCamera(Camera_Move direction)
	{
		if (m_type == CameraType::Fixed || m_type == CameraType::Fixed_Chase) return;

		static float moveSpeed = 50.0f;
		switch (direction) 
		{
		case Camera_Move::FORWARD:
			m_currentPos += m_forward * moveSpeed * timestep;
			break;
		case Camera_Move::BACKWARD:
			m_currentPos -= m_forward * moveSpeed * timestep;
			break;
		case Camera_Move::LEFT:
			m_currentPos-= m_right * moveSpeed * timestep;
			break;
		case Camera_Move::RIGHT:
			m_currentPos += m_right * moveSpeed * timestep;
			break;
		default:
			return;
		}
	}

	void RotateCamera(glm::vec2 coord) 
	{
		if (m_type == CameraType::Fixed || m_type == CameraType::Fixed_Chase) return;

		static float sensitivity = 0.1f;

		coord.x *= sensitivity;
		coord.y *= sensitivity;

		m_yaw += coord.x;
		m_pitch -= coord.y;

		// To avoid screen flip
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		UpdateVectors();
	}

	void UpdateVectors() 
	{
		// Calculate the new Front vector
		glm::vec3 forwardVec;
		forwardVec.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forwardVec.y = sin(glm::radians(m_pitch));
		forwardVec.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_forward = glm::normalize(forwardVec);
		// Also re-calculate the Right and Up vector
		m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors
		m_up = glm::normalize(glm::cross(m_right, m_forward));
	}

	void Fixed_Update() 
	{
		m_currentPos = m_localTrans->GetPosition();

		static glm::vec3 newRot;
		newRot = glm::vec3(0.0, 0.0, 1.0f) * m_localTrans->GetRotation();
		m_forward += newRot - m_entityRot;
		m_entityRot = newRot;
	}

	void Fixed_Chase_Update() 
	{
		m_currentPos = m_currentPos + (m_localTrans->GetPosition() - m_currentPos) * 0.6f;

		m_forward +=  (glm::vec3(0.0, 0.0, 1.0f) * m_localTrans->GetRotation() - m_forward) * 0.6f;
	}

	void Free_Update() 
	{
		
	}

	void Free_Chase_Update() 
	{
		static glm::vec3 newPos;
		newPos = m_localTrans->GetPosition();
		m_currentPos += newPos - m_entityPos;
		m_entityPos = newPos;

		static glm::vec3 newRot;
		newRot = glm::vec3(0.0, 0.0, 1.0f) * m_localTrans->GetRotation();
		m_forward += newRot - m_entityRot;
		m_entityRot = newRot;
	}
};
#endif//!1