#include "VehicleFilterShader.h"

namespace physics
{
	namespace vehicle
	{

		/**
		* Creates a vehicle filter shader which enables physx to decide what stuff car should
		* interact with.
		*/
		physx::PxFilterFlags VehicleFilterShader
		(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* constantBlock, uint32_t constantBlockSize)
		{
			PX_UNUSED(attributes0);
			PX_UNUSED(attributes1);
			PX_UNUSED(constantBlock);
			PX_UNUSED(constantBlockSize);

			if ((0 == (filterData0.word0 & filterData1.word1)) && (0 == (filterData1.word0 & filterData0.word1)))
				return physx::PxFilterFlag::eSUPPRESS;

			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
			pairFlags |= physx::PxPairFlags(uint16_t(filterData0.word2 | filterData1.word2));

			return physx::PxFilterFlags();
		}
	}
}