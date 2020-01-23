#ifndef RAYCAST_H
#define RAYCAST_H

#include "Physics.h"
#include "PXMathUtils.h"

namespace physics
{

	class RayCastManager
	{
		/*PxRaycastHit hitInfo;
		PxU32 maxHits = 1;
		PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eNORMAL | PxHitFlag::eUV;
		PxU32 hitCount = PxGeometryQuery::raycast(origin, unitDir,
			geom, pose,
			maxDist,
			hitFlags,
			maxHits, &hitInfo);*/
	public:

		static void CastRay(glm::vec3 origin, glm::vec3 dir, glm::vec3);

	};

}
#endif //RAYCAST_H

