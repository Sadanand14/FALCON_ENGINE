#ifndef PHYSICS_H
#define PHYSICS_H
#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "boost/container/vector.hpp"

#include "System/Memory/fmemory.h"
#include "Core/Components/TransformComponent.h"
#include "../Log.h"


class Entity;

#define PX_SUPPORT_PVD	
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}


namespace physics
{
	enum PhysicsBodyType
	{
		ESTATIC_BODY,
		EDYNAMIC_BODY,
		EPLANE
	};


	enum PhysicsCollider
	{
		ESPHERE_COLLIDER,
		EBOX_COLLIDER
	};

	bool InitPhysX();
	void CreatePhysicsScene();
	void StepPhysics(float& dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity,const size_t& count);
	bool ShutdownPhysX();

	physx::PxRigidStatic* CreatePlane();
	physx::PxRigidStatic* CreateStaticRigidActor(const Transform* transform, physx::PxShape* collider);
	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform, physx::PxShape* collider);

	physx::PxShape* GetBoxCollider(const float& halfX, const float& halfY, const float& halfZ);
	physx::PxShape* GetSphereCollider(const float& radius);

}

#endif