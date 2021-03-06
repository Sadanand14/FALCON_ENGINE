#ifndef CAMERA_H
#define CAMERA_H

#include <Events/KeyEvents.h>
#include <Events/EventManager.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


enum Camera_Movement{ FORWARD, BACKWARD, LEFT, RIGHT};

// Default Camera Values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();

public:
	float m_Zoom;
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	float m_lastX, m_lastY;

	//Camera Attributes
	//Euler Angles
	float m_Yaw;
	float m_Pitch;
	//Camera Options
	float m_MovementSpeed;
	float m_MouseSensitivity;

	void SetMousePos(glm::vec2 values);
	void SetMouseScroll(glm::vec2 values);

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() const;

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM.
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);
};

#endif
