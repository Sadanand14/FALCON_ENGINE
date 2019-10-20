#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <map>
#include <vector>
#include <Rendering/Mesh.h>
#include <Rendering/Material.h>
#include <glm/glm.hpp>
#include <System/Log.h>
#include <Rendering/Camera.h>

#pragma warning( push )
#pragma warning( disable: 26451 26439 6285)

#include <smart_ptr.hpp>
#include <boost/container/map.hpp>

#pragma warning( pop )

/**
* Stucture to hold Tranform data for each entity.
*/
struct Transform
{
private:
	glm::mat4 m_model; //* The model matrix of the transform
	glm::vec3 m_position; //* The position of the Tranform
	glm::vec3 m_scale; //* The scale of the transform
	glm::quat m_rotation; //* The rotation of the transform

	/**
	 * Recalculates the world matrix
	 */
	void RecalculateMatrix()
	{
		m_model = glm::translate(glm::mat4(1.0f), m_position);
		m_model *= glm::mat4_cast(m_rotation);
		m_model = glm::scale(m_model, m_scale);
	}
public:
	Transform():m_position({ 0.0f, 0.0f, 0.0f }), m_rotation(glm::quat()), m_scale({ 1.0f,1.0f,1.0f}), m_model(1.0f)
	{
		RecalculateMatrix();
	}
	Transform(glm::vec3 pos, glm::quat rot, glm::vec3 scale) : m_position(pos), m_rotation(rot), m_scale(scale)
	{
		RecalculateMatrix();
	}

	inline void SetPosition(const glm::vec3 &pos) { m_position = pos; RecalculateMatrix(); }
	inline void SetRotation(const glm::quat &rot) { m_rotation = rot; RecalculateMatrix(); }
	inline void SetScale(const glm::vec3 &scale) { m_scale = scale; RecalculateMatrix(); }

	inline const glm::vec3 & GetPosition() const { return m_position; }
	inline const glm::quat & GetRotation() const { return m_rotation; }
	inline const glm::vec3 & GetScale() const { return m_scale; }
	inline const glm::mat4 & GetModel() const { return m_model; }
};

/**
* Basic Structure Definition for Components.
*/
struct BasicComponent
{
	BasicComponent() {};
	~BasicComponent() {};
};

/**
*Structure Definition for holding data needed to render the entity.
*/
struct RenderComponent :public BasicComponent
{
	Mesh* m_mesh;

	RenderComponent(): m_mesh(nullptr) {}
	~RenderComponent() {}
};

/**
*Structure Definition for holding data needed for animating the entity.
*/
struct AnimationComponent :public BasicComponent
{

	AnimationComponent() {}
	~AnimationComponent() {}
};

/**
*Structure Definition for holding data needed for calculating physics events on the entity.
*/
struct PhysicsComponent :public BasicComponent
{
	PhysicsComponent() {}
	~PhysicsComponent() {}
};

/**
*Structure definition for holding data needed for playing the relevant audio for the entity.
*/
struct AudioComponent :public BasicComponent
{
	AudioComponent() {}
	~AudioComponent() {}
};

/**
*Structure definition for holding data for AI calculations for the entity.
*/
struct AIComponent :public BasicComponent
{
	AIComponent() {}
	~AIComponent() {}
};

/**
*Structure definition for holding data for Input handling on the entity.
*/
struct InputComponent : public BasicComponent
{
	InputComponent() {}
	~InputComponent() {}
};

/**
*Structure Definition for holding data relevant for getting the view and projection matrices from this entity's location.
*/
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

/**
*Basic Game Object Definition
*/
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
	Entity(glm::vec3 pos, glm::quat rot, glm::vec3 scale):m_transform(Transform(pos, rot, scale)), m_renderC(nullptr), m_audioC(nullptr), m_animationC(nullptr), m_physicsC(nullptr), m_inputC(nullptr), m_AIComponent(nullptr)
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
