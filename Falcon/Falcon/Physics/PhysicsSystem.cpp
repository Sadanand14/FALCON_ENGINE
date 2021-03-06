#include "PhysicsSystem.h"
#include "PhysicsSystem.h"
#include "Core/Components/PhysicsComponent.h"
#include "PxPhysicsAPI.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"



void PhysicsSystem::update(float dt, boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entity, const size_t& entity_count)
{
	glm::vec3 pre_sim_pos, lerp_pos, post_sim_pos;
	glm::quat pre_sim_rot, lerp_rot, post_sim_rot;
	const physx::PxRigidActor* actor;
	const physx::PxShape* collider;
	boost::container::vector<Entity*, fmemory::STLAllocator<Entity*>>* entitySet = entity;
	for (size_t i = 0; i < entity_count; ++i)
	{
		//Update logic
		PhysicsComponent* pc = entitySet->at(i)->GetComponent<PhysicsComponent>();
		if (pc)
		{
			collider = pc->GetCollider();
			actor = collider->getActor();
			if (actor == nullptr) 
			{
				actor = pc->GetActor();
				PXMathUtils::PxVec3toVec3(actor->getGlobalPose().p, post_sim_pos);
				PXMathUtils::PxQuattoQuat(actor->getGlobalPose().q, post_sim_rot);
				
			}
			else //Means shape is not exclusive to this actor
			{
				PXMathUtils::PxVec3toVec3(physx::PxShapeExt::getGlobalPose(*collider, *actor).p, post_sim_pos);
				PXMathUtils::PxQuattoQuat(physx::PxShapeExt::getGlobalPose(*collider, *actor).q, post_sim_rot);
			}


			pre_sim_pos = entitySet->at(i)->GetTransform()->GetPosition();
			pre_sim_rot = entitySet->at(i)->GetTransform()->GetRotation();
			//FL_ENGINE_INFO("New POS= {0},{1},{2}, {3}", post_sim_rot.x, post_sim_rot.y, post_sim_rot.z, post_sim_rot.w);
			lerp_pos = post_sim_pos;//  PXMathUtils::lerp(pre_sim_pos, post_sim_pos, dt);
			lerp_rot = post_sim_rot;// glm::slerp(pre_sim_rot, post_sim_rot, dt);

			entitySet->at(i)->GetTransform()->SetPosition(lerp_pos);
			entitySet->at(i)->GetTransform()->SetRotation(lerp_rot);
		}
	}
	//FL_ENGINE_ERROR("DEBUG AT ME");
}