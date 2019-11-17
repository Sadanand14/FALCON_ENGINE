#ifndef PXMATHCONVERSIONUTIL
#define PXMATHCONVERSIONUTIL


#include "PxPhysicsAPI.h"
#include "Core/Components/TransformComponent.h"
#include <glm/glm.hpp>

class PXMathUtils
{
public:
	static physx::PxVec3* Vec3ToPxVec3(const glm::vec3& vec);
	static physx::PxQuat* Vec3ToPxQuat(const glm::vec3& vec);
	static physx::PxQuat* QuatToPxQuat(const glm::quat& quat);
	static void			  PxVec3toVec3(const physx::PxVec3 vec_s, glm::vec3& vec_d);
	static void			  PxQuattoQuat(const physx::PxQuat quat_s, glm::quat& vec_d);

	inline static glm::vec3 lerp(const glm::vec3& from, const glm::vec3& to, float dt) {return (from + (to - from) * dt);}
};

#endif // !PXMATHCONVERSIONUTIL
