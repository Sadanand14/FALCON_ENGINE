#include <Rendering/Camera.h>
#include <GLFW/glfw3.h>
CameraEventSystem::CameraEventSystem() 
{
	keyCodeVector.push_back(GLFW_KEY_W);
	keyCodeVector.push_back(GLFW_KEY_A);
	keyCodeVector.push_back(GLFW_KEY_S);
	keyCodeVector.push_back(GLFW_KEY_D);
	keyCodeVector.push_back(GLFW_KEY_LEFT_SHIFT);
	keyCodeVector.push_back(GLFW_KEY_LEFT_CONTROL);
	keyCodeVector.push_back(GLFW_KEY_UP);
	keyCodeVector.push_back(GLFW_KEY_DOWN);
	keyCodeVector.push_back(GLFW_KEY_LEFT);
	keyCodeVector.push_back(GLFW_KEY_RIGHT);
}

void CameraEventSystem::SubscribeToEvents()
{
	EventManager::SubscribeToEvent(this, KeyEventCategory);
}

void CameraEventSystem::ProcessEvents() 
{
}

bool CameraEventSystem::CheckForKey(unsigned int code)
{
	bool keyExists = false;
	for (unsigned int i = 0; i < keyCodeVector.size(); i++) 
	{
		if (code == keyCodeVector[i])
			keyExists = true;
	}
	return keyExists;
}

void CameraEventSystem::ProcessKeyEvent(keyType type, unsigned int code)
{
	if (CheckForKey(code)) 
	{
		if (code == GLFW_KEY_W) {}
			//call ProcessKeyBoard of camera Class;
		else if (code == GLFW_KEY_S){}
		else if (code == GLFW_KEY_D){}
		else if (code == GLFW_KEY_A){}
	}
}

/////////////////////////////////////////
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
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

glm::mat4 Camera::GetViewMatrix()
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
	glm::vec3 forwardVec;
	forwardVec.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	forwardVec.y = sin(glm::radians(m_Pitch));
	forwardVec.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Forward = glm::normalize(forwardVec);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));  // Normalize the vectors
	m_Up = glm::normalize(glm::cross(m_Right, m_Forward));
}


