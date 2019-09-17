#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <map>
#include <vector>
#include "..//Rendering/Mesh.h"
#include <glm/glm.hpp>
#include "..//System/Log.h"

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
	glm::vec3 m_position, m_rotation, m_scale;
	Transform():m_position({ 0.0f, 0.0f, 0.0f }), m_rotation({ 0.0f, 0.0f, 0.0f }), m_scale({ 1.0f,1.0f,1.0f})
	{}
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : m_position(pos), m_rotation(rot), m_scale(scale) 
	{}
};

struct BasicComponent 
{
	Status status;
	BasicComponent() :status(Inactive) {}
	~BasicComponent() {};
};

struct RenderComponent :public BasicComponent
{
	Mesh* m_mesh;

	RenderComponent(): m_mesh(nullptr) {}
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


	template<typename F>
	inline void AddComponent() {};

	template<>
	inline void AddComponent<RenderComponent>()
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
	inline void AddComponent<PhysicsComponent>()
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
	inline void AddComponent<AudioComponent>()
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
	inline void AddComponent<AnimationComponent>()
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
	inline void AddComponent<AIComponent>()
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
	inline void AddComponent<InputComponent>()
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

	template<typename T>
	inline T* GetComponent() {};

	template<>
	inline RenderComponent* GetComponent<RenderComponent>() { return m_renderC;}

	template<>
	inline AudioComponent* GetComponent<AudioComponent>() { return m_audioC; }

	template<>
	inline PhysicsComponent* GetComponent<PhysicsComponent>() { return m_physicsC; }

	template<>
	inline AnimationComponent* GetComponent<AnimationComponent>() { return m_animationC; }

	template<>
	inline InputComponent* GetComponent<InputComponent>() { return m_inputC; }

	template<>
	inline AIComponent* GetComponent <AIComponent>() { return m_AIComponent; }
};
#endif // !1
