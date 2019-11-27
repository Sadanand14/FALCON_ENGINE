#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "BasicComponent.h"
#include "System/Physics/Physics.h"
#include "TransformComponent.h"


typedef physx::PxRigidActor Rigidbody;
typedef physx::PxShape      Collider;

/**
*Structure Definition for holding data needed for calculating physics events on the entity.
*/
struct PhysicsComponent: public BasicComponent
{
	
private:
	Rigidbody* actor;
	Collider* collider;

public:
	PhysicsComponent() :
		actor(nullptr),
		collider(nullptr)
	{
		
		/*switch (type)
		{
			case PhysicsBodyType::ERIGID_BODY:
			case PhysicsBodyType::ESTATIC_BODY:
				break;

		}*/
	}

	void SetBoxCollider(const float& halfX, const float& halfY, const float& halfZ)
	{
		collider = physics::GetBoxCollider(halfX, halfY, halfZ);
		if (!collider)
		{
			FL_ENGINE_ERROR("Failed to create box collider");
		}
	}

	void SetSphereCollider(const float& radius)
	{
		if (collider != nullptr)
		{
			FL_ENGINE_ERROR("Collider alread assigned!.");
		}
		collider = physics::GetSphereCollider(radius);
		if (!collider)
		{
			FL_ENGINE_ERROR("Failed to create box collider");
		}
	}

	bool SetPhysicsBodyType(const Transform* transform,physics::PhysicsBodyType type)
	{
		switch (type)
		{
			case physics::PhysicsBodyType::EPLANE:
				actor = physics::CreatePlane();
				break;
			case physics::PhysicsBodyType::EDYNAMIC_BODY:
				actor = physics::CreateDynamicRigidActor(transform,collider);
				break;
			case physics::PhysicsBodyType::ESTATIC_BODY:
				actor = physics::CreateStaticRigidActor(transform,collider);
				break;
		}
		return actor != nullptr;
	}
	~PhysicsComponent() {}

	inline const physx::PxRigidActor* GetActor() { return actor; }
};


#endif // !1
