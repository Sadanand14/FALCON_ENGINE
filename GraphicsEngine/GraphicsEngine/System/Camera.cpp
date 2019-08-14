#include "Camera.h"

//Camera Attributes
glm::vec3 m_Position;
glm::vec3 m_Front;
glm::vec3 m_Up;
glm::vec3 m_Right;
glm::vec3 m_WorldUp;
//Euler Angles
float m_Yaw;
float m_Pitch;
//Camera Options
float m_MovementSpeed;
float m_MouseSensitivity;
float m_Zoom;

glm::mat4 viewMatrix;
//Handles the calculation of view matrix
bool m_dirty;

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch) :
	m_Position(position),
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_WorldUp(worldUp),
	m_Yaw(yaw),
	m_Pitch(pitch),
	m_MovementSpeed(SPEED),
	m_MouseSensitivity(SENSITIVITY),
	m_Zoom(ZOOM),
	m_dirty(true)
{
		UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	m_Position(glm::vec3(posX, posY, posZ)),
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_WorldUp(glm::vec3(upX, upY, upZ)),
	m_Yaw(yaw),
	m_Pitch(pitch),
	m_MovementSpeed(SPEED), 
	m_MouseSensitivity(SENSITIVITY), 
	m_Zoom(ZOOM),
	m_dirty(true)
{	
	UpdateCameraVectors();
}


void Camera::CalculateViewMatrix()
{
	viewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}


glm::mat4 Camera::GetViewMatrix()
{
	if(m_dirty)
		CalculateViewMatrix()
	return viewMatrix;
}

void Camera::Move(Camera_Movement direction, float deltaTime)
{

	float velocity = m_MovementSpeed * deltaTime;

	switch (direction) 
	{
		case FORWARD:
			m_Position += m_Front * velocity;
			break;
		case BACKWARD:
			m_Position -= m_Front * velocity;
			break;
		case LEFT:
			m_Position -= m_Right * velocity;
			break:		
		case RIGHT:
			m_Position += m_Right * velocity;
			break;
	}
	
	m_dirty = true;
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
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 45.0f)
		m_Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
