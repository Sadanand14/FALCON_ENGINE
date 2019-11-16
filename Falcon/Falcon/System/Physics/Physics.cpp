
#include "Physics.h"

#define PVD_HOST "127.0.0.1"

namespace physics
{

	/* PhysX default variables*/

	static physx::PxDefaultAllocator		gAllocator;
	static physx::PxDefaultErrorCallback	gErrorCallback;

	static physx::PxFoundation* gFoundation = NULL;
	static physx::PxPhysics* gPhysics = NULL;

	physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
	physx::PxScene* gScene = NULL;

	physx::PxMaterial* gMaterial = NULL;

	physx::PxPvd* gPvd = NULL;

	bool InitPhysX()
	{
		return false;
	}
	bool ShutdownPhysX()
	{
		return false;
	}
}