#ifndef PHYSICS_H
#define PHYSICS_H
#include <ctype.h>
#include "Types.h"
#include "System/Memory/fmemory.h"
#include "Core/Components/TransformComponent.h"
#include "System/Log.h"
#include "Rendering/BufferDefinitions/VertexLayout.h"


class Entity;
class Mesh;

#define PX_SUPPORT_PVD	
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}


namespace physics
{
	/**
	* This is an enum class specifying body type
	*/
	enum class PhysicsBodyType
	{
		ESTATIC_BODY, /*! < this is static body*/
		EDYNAMIC_BODY,/*! < this is dynamic body*/
		EPLANE
	};

	/**
	* This is an enum class specifying coillider shape
	*/
	enum class PhysicsCollider
	{
		ESPHERE_COLLIDER, /*! < Sphere collider*/
		EBOX_COLLIDER,    /*! < Box collider*/
		ECAPSULE_COLLIDER,/*! < Capsule collider*/
	};

	/*
	* Physics stepping methods.
	*/
	bool InitPhysX();
	void CreatePhysicsScene();
	void StepPhysics(float& dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity,const size_t& count);
	bool ShutdownPhysX();

	/*
	* Physx getter methods.
	*/
	physx::PxPhysics* GetPhysics();
	physx::PxScene* GetPhysicsScene();
	physx::PxCooking* GetCooking();
	physx::PxDefaultAllocator GetAllocator();
	physx::PxMaterial* GetDefaultMaterial();

	/*
	* Rigidbody setup methods.
	*/
	physx::PxRigidStatic* CreatePlane();
	physx::PxRigidStatic* CreateStaticRigidActor(const Transform* transform, physx::PxShape* collider);
	physx::PxRigidDynamic* CreateDynamicRigidActor(const Transform* transform, physx::PxShape* collider);
	physx::PxRigidDynamic* CreateDynamicRigidActor() ;
	void ReleaseCollider(physx::PxRigidActor* ref);

	template<typename T>
	void ReleaseResource(T ref)
	{
		PX_RELEASE(ref);
	}

	/*
	* Collider setup methods.
	*/
	physx::PxShape* GetBoxCollider(const float& halfX, const float& halfY, const float& halfZ);
	physx::PxShape* GetSphereCollider(const float& radius);
	physx::PxShape* GetCapsuleCollider(const float& radius,const float& halfHeight);
	physx::PxShape* GetMeshCollider(const glm::vec3* vertexData, const int& stride, const int& vertCount, glm::vec3 & scaling, bool directInsert = false);
	physx::PxShape* GetMeshColliderWithTriangleMeshes(const glm::vec3* vertexData, const int& vertCount, const int& vertStride,const u32* indexData,
													  const int& indexCount, const int& indexStride, glm::vec3& scaling, bool directInsert = false);
	physx::PxConvexMesh* GetConvexMesh(const glm::vec3* vertexData, const int& stride, const int& vertCount, bool directInsert = false);
	physx::PxShape* GetExclusiveShape(physx::PxRigidActor* actor, const Transform* transform,const glm::vec3* vertexData, const int& count, const int& stride);
	


			

	
}

#endif