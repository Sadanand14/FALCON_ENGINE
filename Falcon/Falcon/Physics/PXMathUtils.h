#ifndef PXMATHCONVERSIONUTIL
#define PXMATHCONVERSIONUTIL


#include "PxPhysicsAPI.h"
#include "Core/Components/TransformComponent.h"
#include <glm/glm.hpp>

/** 
* @class Utiliy class for physx to glm & vice versa conversions.
*/

class PXMathUtils
{
public:
	/**
	* Converts glm vec3 to px vec3.
	* @param ref to source glm::vec3
	* @param ref to destincation phvec3
	*/
	static void           Vec3ToPxVec3(const glm::vec3& vec_s, physx::PxVec3 &vec_d);

	/**
	* Converts glm vec3 to px vec3.
	* @param ref to source glm::vec3
	* @return pointer destincation pxvec3
	*/
	static physx::PxVec3* Vec3ToPxVec3(const glm::vec3& vec);

	/**
	* Converts glm vec3 to px quat.
	* @param ref to source glm::vec3
	* @param ref to destincation pxquat
	*/
	static physx::PxQuat* Vec3ToPxQuat(const glm::vec3& vec);

	/**
	* Converts glm quat to px quat
	* @param ref to source glm::quat
	* @param pointer to destincation phquat
	*/
	static physx::PxQuat* QuatToPxQuat(const glm::quat& quat);
	
	/**
	* Converts px vec3 to glm vec3.
	* @param ref to source pxvec3
	* @param ref to destincation px vec3
	*/
	static void PxVec3toVec3(const physx::PxVec3 vec_s, glm::vec3& vec_d);
	
	/**
	* Converts px quat to glm quat.
	* @param ref to source px quat
	* @param ref to destincation glm::quat
	*/
	static void	PxQuattoQuat(const physx::PxQuat quat_s, glm::quat& vec_d);

	/**
	* lerp for vec3
	* @param starting glm::vec3
	* @param ending glm::vec3
	* @param float dt
	*/
	inline static glm::vec3 lerp(const glm::vec3& from, const glm::vec3& to, float dt) {return (from + (to - from) * dt);}
	
	/**
	* Slerp for quat
	* @param starting glm::quat
	* @param ending glm::quat
	* @param float dt
	*/
	inline static glm::quat slerp(const glm::quat& from, const glm::quat& to, float dt) { return (from + (to - from) * dt); }
};

#endif // !PXMATHCONVERSIONUTIL
