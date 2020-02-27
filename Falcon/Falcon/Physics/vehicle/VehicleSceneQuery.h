#ifndef VEHICLESCENEQUERY_H
#define VEHICLESCENEQUERY_H

#include "../Physics.h"

namespace physics {

	namespace vehicle
	{

		enum
		{
			DRIVABLE_SURFACE = 0xffff0000,
			UNDRIVABLE_SURFACE = 0x0000ffff
		};

		void setupDrivableSurface(physx::PxFilterData& filterData);

		void setupNonDrivableSurface(physx::PxFilterData& filterData);

		physx::PxQueryHitType::Enum WheelSceneQueryPreFilterBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			physx::PxHitFlags& queryFlags);

		physx::PxQueryHitType::Enum WheelSceneQueryPostFilterBlocking
		(physx::PxFilterData queryFilterData, physx::PxFilterData objectFilterData,
			const void* constantBlock, uint32_t constantBlockSize,
			const physx::PxQueryHit& hit);

		physx::PxQueryHitType::Enum WheelSceneQueryPreFilterNonBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			const physx::PxHitFlags& queryFlags);

		physx::PxQueryHitType::Enum WheelSceneQueryPostFilterNonBlocking
		(physx::PxFilterData queryFilterData, physx::PxFilterData objectFilterData,
			const void* constantBlock, uint32_t constantBlockSize,
			const physx::PxQueryHit& hit);

		class VehicleSceneQueryData
		{
		public:
			VehicleSceneQueryData();
			~VehicleSceneQueryData();

			//Allocate scene query data for up to maxNumVehicles and up to maxNumWheelsPerVehicle with numVehiclesInBatch per batch query.
			static VehicleSceneQueryData* allocate
			(const uint32_t maxNumVehicles, const uint32_t maxNumWheelsPerVehicle, const uint32_t maxNumHitPointsPerWheel, const uint32_t numVehiclesInBatch,
				physx::PxBatchQueryPreFilterShader preFilterShader,physx::PxBatchQueryPostFilterShader postFilterShader,
				physx::PxAllocatorCallback& allocator);

			//Free allocated buffers.
			void free(physx::PxAllocatorCallback& allocator);

			//Create a PxBatchQuery instance that will be used for a single specified batch.
			static physx::PxBatchQuery* setUpBatchedSceneQuery(const uint32_t batchId, const VehicleSceneQueryData& vehicleSceneQueryData, physx::PxScene* scene);

			//Return an array of scene query results for a single specified batch.
			physx::PxRaycastQueryResult* getRaycastQueryResultBuffer(const uint32_t batchId);

			//Return an array of scene query results for a single specified batch.
			physx::PxSweepQueryResult* getSweepQueryResultBuffer(const uint32_t batchId);

			//Get the number of scene query results that have been allocated for a single batch.
			uint32_t getQueryResultBufferSize() const;

		private:

			//Number of queries per batch
			uint32_t mNumQueriesPerBatch;

			//Number of hit results per query
			uint32_t mNumHitResultsPerQuery;

			//One result for each wheel.
			physx::PxRaycastQueryResult* mRaycastResults;
			physx::PxSweepQueryResult* mSweepResults;

			//One hit for each wheel.
			physx::PxRaycastHit* mRaycastHitBuffer;
			physx::PxSweepHit* mSweepHitBuffer;

			//Filter shader used to filter drivable and non-drivable surfaces
			physx::PxBatchQueryPreFilterShader mPreFilterShader;

			//Filter shader used to reject hit shapes that initially overlap sweeps.
			physx::PxBatchQueryPostFilterShader mPostFilterShader;

		};
	}
} 
#endif //VEHICLESCENEQUERY_H