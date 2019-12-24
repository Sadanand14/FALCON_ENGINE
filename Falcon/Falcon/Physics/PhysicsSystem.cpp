#include "PhysicsSystem.h"
#include "PhysicsSystem.h"
#include "Core/Components/PhysicsComponent.h"
#include "PxPhysicsAPI.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"



void PhysicsSystem::update(float dt, boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>>* entity, const size_t& entity_count)
{
	glm::vec3 pre_sim_pos, lerp_pos,  post_sim_pos;
	glm::quat pre_sim_rot,lerp_rot, post_sim_rot;
	const physx::PxRigidActor* actor;
	boost::container::vector<Entity*, fmemory::StackSTLAllocator<Entity*>> entitySet = *entity;
	for (size_t i = 1; i < entity_count; ++i)
	{
		//Update logic
		actor = entitySet[i]->GetComponent<PhysicsComponent>()->GetActor();;
		PXMathUtils::PxVec3toVec3(actor->getGlobalPose().p, post_sim_pos);
		PXMathUtils::PxQuattoQuat(actor->getGlobalPose().q, post_sim_rot);
		pre_sim_pos = entitySet[i]->GetTransform()->GetPosition();
		pre_sim_rot = entitySet[i]->GetTransform()->GetRotation();
		FL_ENGINE_INFO("New POS= {0},{1},{2}", post_sim_pos.x, post_sim_pos.y, post_sim_pos.z);
		lerp_pos = PXMathUtils::lerp(pre_sim_pos, post_sim_pos, dt);
		lerp_rot = glm::slerp(pre_sim_rot, post_sim_rot, dt);

		entitySet[i]->GetTransform()->SetPosition(lerp_pos);
		entitySet[i]->GetTransform()->SetRotation(lerp_rot);
	}
}
