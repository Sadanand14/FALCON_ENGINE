#ifndef PHYSICS_H
#define PHYSICS_H
#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "boost/container/vector.hpp"

#include "System/Memory/fmemory.h"
#include "Core/Components/TransformComponent.h"
#include "System/Log.h"
#include "glm/vec3.hpp"
//#include "Rendering/BufferDefinitions/VertexLayout.h"

class Entity;

#define PX_SUPPORT_PVD	
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}


namespace physics
{
	/**
	* This is an enum class specifying body type
	*/
	enum PhysicsBodyType
	{
		ESTATIC_BODY, /*! < this is static body*/
		EDYNAMIC_BODY,/*! < this is dynamic body*/
		EPLANE
	};

	/**
	* This is an enum class specifying coillider shape
	*/
	enum PhysicsCollider
	{
		ESPHERE_COLLIDER, /*! < Sphere collider*/
		EBOX_COLLIDER,    /*! < Box collider*/
		ECAPSULE_COLLIDER,/*! < Capsule collider*/
	};

	bool InitPhysX();
	void CreatePhysicsScene();
	void StepPhysics(float& dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entity,const size_t& count);
	bool ShutdownPhysX();

	physx::PxRigidStatic* CreatePlane();
	physx::PxRigidStatic* CreateStaticRigidActor(const Transform* transform, physx::PxShape* collider);
	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform, physx::PxShape* collider);

	physx::PxShape* GetBoxCollider(const float& halfX, const float& halfY, const float& halfZ);
	physx::PxShape* GetSphereCollider(const float& radius);
	physx::PxShape* GetCapsuleCollider(const float& radius,const float& halfHeight);
	physx::PxShape* GetMeshCollider(const glm::vec3* vertexData, const int& stride, const int& vertCount, bool directInsert = false);

}

#endif