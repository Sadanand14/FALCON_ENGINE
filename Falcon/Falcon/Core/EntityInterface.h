#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <map>
#include <vector>
#include <Types.h>
#include <PipeLine/Material.h>
#include <Log.h>


#include <Components/RenderComponent.h>
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
	Transform* m_transform = nullptr;
	RenderComponent* m_renderC = nullptr;
	AudioComponent* m_audioC = nullptr;
	PhysicsComponent* m_physicsC = nullptr;
	InputComponent* m_inputC = nullptr;
	AnimationComponent* m_animationC = nullptr;
	AIComponent* m_AIComponent = nullptr;
	ParticleEmitterComponent* m_particleEmitterC = nullptr;
	CameraComponent* m_cameraC = nullptr;

public:

	Entity()
	{
		m_transform = fmemory::fnew<Transform>();
	}
	Entity(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
	{
		m_transform = fmemory::fnew<Transform>(pos, rot, scale);
	}
	~Entity()
	{
		if(m_transform)
			fmemory::fdelete<>(m_transform);
		if(m_renderC)
			fmemory::fdelete<>(m_renderC);
		if(m_audioC)
			fmemory::fdelete<>(m_audioC);
		if(m_physicsC)
			fmemory::fdelete<>(m_physicsC);
		if(m_animationC)
			fmemory::fdelete<>(m_animationC);
		if(m_AIComponent)
			fmemory::fdelete<>(m_AIComponent);
		if(m_inputC)
			fmemory::fdelete<>(m_inputC);
		if(m_particleEmitterC)
			fmemory::fdelete<>(m_particleEmitterC);
		if (m_cameraC)
			fmemory::fdelete(m_cameraC);
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
		m_particleEmitterC = fmemory::fnew<ParticleEmitterComponent>();
	}
}

template<>
inline void Entity::AddComponent<CameraComponent>() 
{
	if (m_cameraC) 
	{
		FL_ENGINE_ERROR("This entity already has this component!");
	}
	else 
	{
		m_cameraC = fmemory::fnew<CameraComponent>(m_transform);
	}
}


template<>
inline CameraComponent* Entity::GetComponent<CameraComponent>() { return m_cameraC; }

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
