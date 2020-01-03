#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <map>
#include <vector>
#include <PipeLine/Material.h>
#include <glm/glm.hpp>
#include <Log.h>


#include <Components/RenderCcomponent.h>
#include <Components/TransformComponent.h>
#include <Components/AnimationComponent.h>
#include <Components/AudioComponent.h>
#include <Components/PhysicsComponent.h>
#include <Components/InputComponent.h>
#include <Components/AIComponent.h>
#include <Components/CameraComponent.h>
#include <Components/ParticleEmitterComponent.h>

#pragma warning( push )
#pragma warning( disable: 26451 26439 6285)

#include <smart_ptr.hpp>
#include <boost/container/map.hpp>

#pragma warning( pop )

/*
	TODO: Make use of custom allocators
*/

/**
*Basic Game Object Definition
*/
class Entity
{
private:
	Transform* m_transform;
	RenderComponent* m_renderC;
	AudioComponent* m_audioC;
	PhysicsComponent* m_physicsC;
	InputComponent* m_inputC;
	AnimationComponent* m_animationC;
	AIComponent* m_AIComponent;
	ParticleEmitterComponent* m_particleEmitterC;

public:

	Entity()
		:m_renderC(nullptr), m_audioC(nullptr), m_animationC(nullptr), m_physicsC(nullptr), m_inputC(nullptr),
		m_AIComponent(nullptr), m_particleEmitterC(nullptr)
	{
		m_transform = fmemory::fnew<Transform>();
	}
	Entity(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
		: m_renderC(nullptr), m_audioC(nullptr), m_animationC(nullptr), m_physicsC(nullptr), m_inputC(nullptr),
		m_AIComponent(nullptr), m_particleEmitterC(nullptr)
	{
		m_transform = fmemory::fnew<Transform>(pos, rot, scale);
	}
	~Entity()
	{
		fmemory::fdelete<Transform>(m_transform);
		fmemory::fdelete<RenderComponent>(m_renderC);
		fmemory::fdelete<AudioComponent>(m_audioC);
		fmemory::fdelete<PhysicsComponent>(m_physicsC);
		fmemory::fdelete<AnimationComponent>(m_animationC);
		fmemory::fdelete<AIComponent>(m_AIComponent);
		fmemory::fdelete<InputComponent>(m_inputC);
	}

	inline Transform* GetTransform() { return m_transform; }

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
		m_renderC = fmemory::fnew<RenderComponent>();
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
		m_physicsC = fmemory::fnew<PhysicsComponent>();
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
		m_audioC = fmemory::fnew<AudioComponent>();
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
		m_animationC = fmemory::fnew<AnimationComponent>();
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
		m_AIComponent = fmemory::fnew<AIComponent>();
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
		m_inputC = fmemory::fnew <InputComponent>();
	}
}

template<>
inline void Entity::AddComponent<ParticleEmitterComponent>()
{
	if (m_particleEmitterC)
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else
	{
		m_particleEmitterC = new ParticleEmitterComponent();
	}
}

template<>
inline RenderComponent* Entity::GetComponent<RenderComponent>() { return m_renderC; }

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

template<>
inline ParticleEmitterComponent* Entity::GetComponent<ParticleEmitterComponent>() { return m_particleEmitterC; }

#endif // !1
