#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "BasicComponent.h"
#include "System/Physics/Physics.h"
#include "TransformComponent.h"

enum PhysicsBodyType
{
	ESTATIC_BODY,
	ERIGID_BODY
};

/**
*Structure Definition for holding data needed for calculating physics events on the entity.
*/
struct PhysicsComponent: public BasicComponent
{
	
private:
	physx::PxRigidActor* actor;

public:
	PhysicsComponent(const Transform* transform):
		actor(nullptr) 
	{
		actor = physics::CreateDynamicRigidActor(transform);
		/*switch (type)
		{
			case PhysicsBodyType::ERIGID_BODY:
			case PhysicsBodyType::ESTATIC_BODY:
				break;

		}*/
	}
	~PhysicsComponent() {}

	inline const physx::PxRigidActor* GetActor() { return actor; }
};


#endif // !1
