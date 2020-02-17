#include "Camera.h"
#include <GLFW/glfw3.h>

/////////////////////////////////////////
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM),
																			m_lastX(0), m_lastY(0)
{
	m_Position = position;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	UpdateCameraVectors();
}

void Camera::SetMousePos(glm::vec2 value) 
{
	ProcessMouseMovement(value.x -m_lastX , m_lastY-value.y);
	m_lastX = value.x;
	m_lastY = value.y;
}

void Camera::SetMouseScroll(glm::vec2 value) 
{
	ProcessMouseScroll(value.y);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_Position += m_Forward * velocity;
	if (direction == BACKWARD)
		m_Position -= m_Forward * velocity;
	if (direction == LEFT)
		m_Position -= m_Right * velocity;
	if (direction == RIGHT)
		m_Position += m_Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// To avoid screen flip
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
	{if(yoffset==0) return;
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	else if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	else if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 forwardVec;
	forwardVec.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	forwardVec.y = sin(glm::radians(m_Pitch));
	forwardVec.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Forward = glm::normalize(forwardVec);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));  // Normalize the vectors
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}


