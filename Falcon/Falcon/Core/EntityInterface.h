#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <map>
#include <vector>
#include "..//Rendering/Mesh.h"
#include "../Rendering/Material.h"
#include <glm/glm.hpp>
#include "..//System/Log.h"
#include "..//Rendering/Camera.h"

#pragma warning( push )
#pragma warning( disable: 26451 26439 6285)

#include <smart_ptr.hpp>
#include <boost/container/map.hpp>

#pragma warning( pop )

//Enumeration for the type of compoenents we require for our entities
//enum ComponentType{Rendering, Audio, Animation, Input, Physics, AI};

enum Status { Inactive, Active};

struct Transform
{
private:
	glm::mat4 m_model;
	glm::vec3 m_position, m_rotation, m_scale;

	void RecalculateMatrix()
	{
		m_model = glm::translate(glm::mat4(1.0f), m_position);
		m_model = glm::scale(m_model, m_scale);
	}
public:
	Transform():m_position({ 0.0f, 0.0f, 0.0f }), m_rotation({ 0.0f, 0.0f, 0.0f }), m_scale({ 1.0f,1.0f,1.0f}), m_model(1.0f)
	{}
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : m_position(pos), m_rotation(rot), m_scale(scale)
	{}

	inline void SetPosition(const glm::vec3 &pos) { m_position = pos; RecalculateMatrix(); }
	inline void SetRotation(const glm::vec3 &rot) { m_rotation = rot; RecalculateMatrix(); }
	inline void SetScale(const glm::vec3 &scale) { m_scale = scale; RecalculateMatrix(); }

	inline const glm::vec3 & GetPosition() const { return m_position; }
	inline const glm::vec3 & GetRotation() const { return m_rotation; }
	inline const glm::vec3 & GetScale() const { return m_scale; }
	inline const glm::mat4 & GetModel() const { return m_model; }
};

struct BasicComponent
{
	BasicComponent() {};
	~BasicComponent() {};
};

struct RenderComponent :public BasicComponent
{
	Mesh* m_mesh;
	Material* m_material;

	RenderComponent(): m_mesh(nullptr), m_material(nullptr) {}
	~RenderComponent() {}
};

struct AnimationComponent :public BasicComponent
{

	AnimationComponent() {}
	~AnimationComponent() {}
};

struct PhysicsComponent :public BasicComponent
{
	PhysicsComponent() {}
	~PhysicsComponent() {}
};

struct AudioComponent :public BasicComponent
{
	AudioComponent() {}
	~AudioComponent() {}
};

struct AIComponent :public BasicComponent
{
	AIComponent() {}
	~AIComponent() {}
};

struct InputComponent : public BasicComponent
{
	InputComponent() {}
	~InputComponent() {}
};

struct CameraComponent : public BasicComponent
{
private:
	Camera* m_camera;
public:
	CameraComponent(Transform transform)
	{
		m_camera->m_Position = transform.GetPosition();
		//m_camera->m_Position = transform.GetRotation();
		//m_camera->m_Position = transform.GetScale();
	};
	~CameraComponent() {};
};

class Entity
{
private:
	Transform m_transform;
	RenderComponent* m_renderC;
	AudioComponent* m_audioC;
	PhysicsComponent* m_physicsC;
	InputComponent* m_inputC;
	AnimationComponent* m_animationC;
	AIComponent* m_AIComponent;

public:

	Entity() : m_transform(Transform()) , m_renderC(nullptr), m_audioC(nullptr), m_animationC(nullptr), m_physicsC(nullptr), m_inputC(nullptr), m_AIComponent(nullptr)
	{}
	Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale):m_transform(Transform(pos, rot, scale)), m_renderC(nullptr), m_audioC(nullptr), m_animationC(nullptr), m_physicsC(nullptr), m_inputC(nullptr), m_AIComponent(nullptr)
	{}
	~Entity() {}

	inline Transform & GetTransform() { return m_transform; }

	template<typename F>
	inline void AddComponent() {};


	template<typename T>
	inline T* GetComponent() {};
};


template<>
inline void Entity::AddComponent<RenderComponent>()
{
	if (m_renderC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_renderC = new RenderComponent();
	}
}

template<>
inline void Entity::AddComponent<PhysicsComponent>()
{
	if (m_physicsC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_physicsC = new PhysicsComponent();
	}
}

template<>
inline void Entity::AddComponent<AudioComponent>()
{
	if (m_audioC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_audioC = new AudioComponent();
	}
}

template<>
inline void Entity::AddComponent<AnimationComponent>()
{
	if (m_animationC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_animationC = new AnimationComponent();
	}
}

template<>
inline void Entity::AddComponent<AIComponent>()
{
	if (m_AIComponent)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_AIComponent = new AIComponent();
	}
}

template<>
inline void Entity::AddComponent<InputComponent>()
{
	if (m_inputC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_inputC = new InputComponent();
	}
}

template<>
inline RenderComponent* Entity::GetComponent<RenderComponent>() { return m_renderC;}

template<>
inline AudioComponent* Entity::GetComponent<AudioComponent>() { return m_audioC; }

template<>
inline PhysicsComponent* Entity::GetComponent<PhysicsComponent>() { return m_physicsC; }

template<>
inline AnimationComponent* Entity::GetComponent<AnimationComponent>() { return m_animationC; }

template<>
inline InputComponent* Entity::GetComponent<InputComponent>() { return m_inputC; }

template<>
inline AIComponent* Entity::GetComponent <AIComponent>() { return m_AIComponent; }

#endif // !1
