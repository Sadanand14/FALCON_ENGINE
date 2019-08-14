#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT,UP,DOWN };

// Default Camera Values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM_DEFAULT = 45.0f;

class Camera
{
private:
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

	glm::mat4 m_ViewMatrix;
	//Handles the calculation of view matrix
	bool m_dirty;

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors();
	void CalculateViewMatrix();


public:



	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float& posX, float& posY, float& posZ, float& upX, float& upY, float& upZ, float& yaw, float& pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();


	// Repositioning logic
	inline glm::vec3 GetPosition() const { return m_Position; }
	inline void SetPosition(glm::vec3& position) { m_Position = position; }
	void Move(Camera_Movement direction, float deltaTime);
	//void MoveRelative(glm::vec3 displacement);


	// Rotation logic
	void Rotate(float& xoffset, float& yoffset, GLboolean constrainPitch = true);

	// Zoom logic
	void Zoom(float& yoffset);
	inline float GetZoom () const { return m_Zoom; }
};

#endif
