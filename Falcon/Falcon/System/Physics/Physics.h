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
	bool InitPhysX();
	void CreatePhysicsScene();
	void StepPhysics(float& dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity,const size_t& count);
	bool ShutdownPhysX();

	void CreatePlane();
	physx::PxRigidStatic* CreateStaticRigidActor(const Transform* transform);
	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform);


}

#endif