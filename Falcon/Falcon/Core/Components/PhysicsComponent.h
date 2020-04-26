#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "BasicComponent.h"
#include "Physics/Physics.h"
#include "Physics/vehicle/Vehicle.h"
#include "TransformComponent.h"
#include "glm/vec3.hpp"

typedef physx::PxRigidActor   Rigidbody;
typedef physx::PxRigidDynamic RigidbodyDynamic;
typedef physx::PxRigidStatic  RigidbodyStatic;
typedef physx::PxShape        Collider;
typedef physx::PxConvexMesh   ConvexMesh;
/**
*Structure Definition for holding data needed for calculating physics events on the entity.
*/
struct PhysicsComponent: public BasicComponent
{
	
private:
	Rigidbody* m_actor;
	Collider* m_collider;

public:
	PhysicsComponent() :
		m_actor(nullptr),
		m_collider(nullptr)
		
	{
		
		/*switch (type)
		{
			case PhysicsBodyType::ERIGID_BODY:
			case PhysicsBodyType::ESTATIC_BODY:
				break;

		}*/
	}

	PhysicsComponent(Rigidbody* actor) :
		m_actor(nullptr),
		m_collider(nullptr)
	
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
		if (m_collider != nullptr)
		{
			FL_ENGINE_ERROR("{0} Collider already assigned!. ",m_collider->getGeometryType());
			return;
		}
		
		m_collider = physics::GetBoxCollider(halfX, halfY, halfZ);
		if (!m_collider)
		{
			FL_ENGINE_ERROR("Failed to create box m_collider");
		}
	}


	void SetSphereCollider(const float& radius)
	{
		if (m_collider != nullptr)
		{
			FL_ENGINE_ERROR("{0} Collider already assigned!. ", m_collider->getGeometryType());
			return;
		}
		
		m_collider = physics::GetSphereCollider(radius);
		if (!m_collider)
		{
			FL_ENGINE_ERROR("Failed to create sphere m_collider");
		}
	}

	void SetCapsuleCollider(const float& radius, const float& halfHeight)
	{
		if (m_collider != nullptr)
		{
			FL_ENGINE_ERROR("{0} Collider already assigned!. ", m_collider->getGeometryType());
			return;
		}

		m_collider = physics::GetCapsuleCollider(radius,halfHeight);
		if (!m_collider)
		{
			FL_ENGINE_ERROR("Failed to create capsule m_collider");
		}
	}

	void SetMeshCollider(const glm::vec3* vertexData, const int& count, const int& stride, glm::vec3& scale)
	{

		if (m_collider != nullptr)
		{
			FL_ENGINE_ERROR("{0} Collider already assigned!. ", m_collider->getGeometryType());
			return;
		}

		m_collider = physics::GetMeshCollider(vertexData, stride, count,scale);
		if (!m_collider)
		{
			FL_ENGINE_ERROR("Failed to create mesh m_collider");
		}
	}


	void SetMeshColliderWithTriangleMeshes(const glm::vec3* vertexData, const int& vertcount, const int& vertexStride,
										  const u32* indexData, const int& indexCount, const int& indexStride, glm::vec3& scale)
	{
		if (m_collider != nullptr)
		{
			FL_ENGINE_ERROR("{0} Collider already assigned!. ", m_collider->getGeometryType());
			return;
		}

		m_collider = physics::GetMeshColliderWithTriangleMeshes(vertexData, vertcount,  vertexStride, 
																indexData, indexCount, indexStride,scale);

		if (!m_collider)
		{
			FL_ENGINE_ERROR("Failed to create mesh m_collider");
		}
	}


	void SetCollider(Collider* m_collider)
	{
		if (m_collider == nullptr)
		{
			FL_ENGINE_WARN("WARNING: Received a nullptr for m_collider.");
			return;
		}
		m_collider = m_collider;
	}

	bool SetPhysicsBodyType(const Transform* transform,physics::PhysicsBodyType type)
	{
		switch (type)
		{
			case physics::PhysicsBodyType::EPLANE:
				m_actor = physics::CreatePlane();
				break;
			case physics::PhysicsBodyType::EDYNAMIC_BODY:
				m_actor = physics::CreateDynamicRigidActor(transform,m_collider);
				break;
			case physics::PhysicsBodyType::ESTATIC_BODY:
				m_actor = physics::CreateStaticRigidActor(transform,m_collider);
				break;
		}
		return m_actor != nullptr;
	}

	void AddToExclusiveShape(Rigidbody* actor, const Transform* transform,const glm::vec3* vertexData, const int& count, const int& stride)
	{
		m_actor = actor;
		m_collider = physics::GetExclusiveShape(actor,transform,vertexData,count,stride);
	}


	inline const Collider* GetCollider() const { return m_collider; }
	inline const Rigidbody* GetActor() const  { return m_actor; }


	void MakeDrivableSurface()
	{
		if (m_collider == nullptr)
		{
			FL_ENGINE_INFO("INFO: Can't make null collider shape a drivable surface");
			return;
		}
		else
		{
			physics::vehicle::MakeActorDrivableSurface(m_collider);
		}
	}


	~PhysicsComponent()
	{
		//physics::ReleaseCollider(m_actor);
		//physics::ReleaseResource(m_collider);
	}

};


#endif // !1
