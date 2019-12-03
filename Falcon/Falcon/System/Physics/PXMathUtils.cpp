#include "PXMathUtils.h"

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"
#include "Memory/fmemory.h"

void PXMathUtils::Vec3ToPxVec3(const glm::vec3& vec_s, physx::PxVec3& vec_d)
{
	/// TODO: Use memcopy insted
	vec_d.x = vec_s.x;
	vec_d.y = vec_s.y;
	vec_d.z = vec_s.z;
}


physx::PxVec3* PXMathUtils::Vec3ToPxVec3(const glm::vec3& vec)
{
	/// TODO: Use memcopy insted
	return fmemory::fnew<physx::PxVec3>(vec.x, vec.y, vec.z);
}

physx::PxQuat* PXMathUtils::Vec3ToPxQuat(const glm::vec3& vec)
{
	/// TODO: Use memcopy insted
	return fmemory::fnew<physx::PxQuat>(vec.x, vec.y, vec.z, 0.0f);
}

physx::PxQuat* PXMathUtils::QuatToPxQuat(const glm::quat& quat)
{
	/// TODO: Use memcopy insted
	return  fmemory::fnew<physx::PxQuat>(quat.x, quat.y, quat.z, (quat.w == 0) ? 1.0f : quat.w);
}

void PXMathUtils::PxVec3toVec3(const physx::PxVec3 vec_s, glm::vec3& vec_d)
{
	/// TODO: Use memcopy insted
	vec_d.x = vec_s.x;
	vec_d.y = vec_s.y;
	vec_d.z = vec_s.z;
}

void PXMathUtils::PxQuattoQuat(const physx::PxQuat quat_s, glm::quat& quat_d)
{

	/// TODO: Use memcopy insted
	quat_d.x = quat_s.x;
	quat_d.y = quat_s.y;
	quat_d.z = quat_s.z;
	quat_d.w = quat_s.w;
}
