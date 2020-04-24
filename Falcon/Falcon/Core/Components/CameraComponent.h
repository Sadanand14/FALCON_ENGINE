#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include "BasicComponent.h"
#include "TransformComponent.h"
#include <Types.h>
#include <CameraEvent.h>
#include <EventManager.h>
#include <cmath>

enum class Camera_Move { FORWARD, BACKWARD, LEFT, RIGHT };
enum class CameraType { Free, Fixed, Fixed_Chase, Free_Chase };

/**
*Structure Definition for holding data relevant for getting the view from this entity's location.
*/
struct CameraComponent : public BasicComponent
{
private:
	CameraType m_type;
	glm::vec3 m_camPos, m_camForward, m_camUp, m_camRight, m_forward, m_transPos;
	glm::quat m_transRot;
	glm::mat4 m_view, m_camSpace;
	float m_yaw, m_pitch, m_offsetMagX, m_offsetMagY;
	Transform* m_localTrans;
	float timestep;

public:

	inline CameraType GetType() { return m_type; }

	CameraComponent(Transform* transform)
		:m_offsetMagX(-10.0f), m_offsetMagY(5.0f), m_localTrans(transform), m_type(CameraType::Free), m_yaw(-90.0f),
		m_pitch(0.0f)
	{
		m_forward = glm::vec3(0.0f, 0.0f, 1.0f) * glm::inverse(m_localTrans->GetRotation());
		m_camPos = glm::vec3(0.0f,2.0f,-4.0f);

		UpdateVectors();
		EventManager::PushEvent(boost::make_shared <CameraEvent>(this), EVENT_CAMERA_COMPONENT);
	}
	~CameraComponent() {}

	inline glm::vec3 GetPos() { return m_camPos; }
	inline glm::mat4 GetViewMatrix() const
	{
		return m_view;
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
			m_camPos += m_camForward * moveSpeed * timestep;
			break;
		case Camera_Move::BACKWARD:
			m_camPos -= m_camForward * moveSpeed * timestep;
			break;
		case Camera_Move::LEFT:
			m_camPos -= m_camRight * moveSpeed * timestep;
			break;
		case Camera_Move::RIGHT:
			m_camPos += m_camRight * moveSpeed * timestep;
			break;
		default:
			return;
		}
	}

	void RotateCamera(glm::vec2 coord)
	{
		//if (m_type == CameraType::Fixed || m_type == CameraType::Fixed_Chase) return;

		static float sensitivity = 1.0f;

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
		static glm::vec3 prevforward = { 0.0f,0.0f,0.0f };

		glm::vec3 forwardVec;
		forwardVec.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forwardVec.y = sin(glm::radians(m_pitch));
		forwardVec.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forwardVec = glm::normalize(forwardVec);
		
		//if(glm::length(diff)!=0)
		m_camForward = forwardVec;

		// Also re-calculate the Right and Up vector
		m_camRight = glm::normalize(glm::cross(m_camForward, glm::vec3(0.0f, 1.0f, 0.0f)));// Normalize the vectors
		m_camUp = glm::normalize(glm::cross(m_camRight, m_camForward));
	}

	void GetSpace() 
	{
		m_camSpace = glm::translate(m_localTrans->GetParent(), m_localTrans->GetPosition());
		m_camSpace *= glm::mat4_cast(m_localTrans->GetRotation());
		m_camSpace = glm::inverse(m_camSpace);
	}

	void Fixed_Update()
	{
		GetSpace();
		m_view =  glm::lookAt(m_camPos, m_camPos + m_camForward, m_camUp)* m_camSpace;
	}

	void Fixed_Chase_Update()
	{
	
	}

	void Free_Update()
	{
		m_view = glm::lookAt(m_camPos, m_camPos + m_camForward, m_camUp);
	}

	void Free_Chase_Update()
	{
		GetSpace();
		m_view = glm::lookAt(m_camPos, m_camPos + m_camForward, m_camUp) * m_camSpace;
	}
};
#endif//!1