#include "VehicleSceneQuery.h"


namespace physics
{

	namespace vehicle
	{
		void setupDrivableSurface(physx::PxFilterData& filterData)
		{
			filterData.word3 = static_cast<uint32_t>(DRIVABLE_SURFACE);
		}

		void setupNonDrivableSurface(physx::PxFilterData& filterData)
		{
			filterData.word3 = UNDRIVABLE_SURFACE;
		}

		physx::PxQueryHitType::Enum WheelSceneQueryPreFilterBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			physx::PxHitFlags& queryFlags)
		{
			//filterData0 is the vehicle suspension query.
			//filterData1 is the shape potentially hit by the query.
			PX_UNUSED(filterData0);
			PX_UNUSED(constantBlock);
			PX_UNUSED(constantBlockSize);
			PX_UNUSED(queryFlags);
			return ((0 == (filterData1.word3 & DRIVABLE_SURFACE)) ? physx::PxQueryHitType::eNONE : physx::PxQueryHitType::eBLOCK);
		}

		physx::PxQueryHitType::Enum WheelSceneQueryPostFilterBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			const physx::PxQueryHit& hit)
		{
			PX_UNUSED(filterData0);
			PX_UNUSED(filterData1);
			PX_UNUSED(constantBlock);
			PX_UNUSED(constantBlockSize);
			if ((static_cast<const physx::PxSweepHit&>(hit)).hadInitialOverlap())
				return physx::PxQueryHitType::eNONE;
			return physx::PxQueryHitType::eBLOCK;
		}

		physx::PxQueryHitType::Enum WheelSceneQueryPreFilterNonBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			physx::PxHitFlags& queryFlags)
		{
			//filterData0 is the vehicle suspension query.
			//filterData1 is the shape potentially hit by the query.
			PX_UNUSED(filterData0);
			PX_UNUSED(constantBlock);
			PX_UNUSED(constantBlockSize);
			PX_UNUSED(queryFlags);
			return ((0 == (filterData1.word3 & DRIVABLE_SURFACE)) ? physx::PxQueryHitType::eNONE : physx::PxQueryHitType::eTOUCH);
		}

		physx::PxQueryHitType::Enum WheelSceneQueryPostFilterNonBlocking
		(physx::PxFilterData filterData0, physx::PxFilterData filterData1,
			const void* constantBlock, uint32_t constantBlockSize,
			const physx::PxQueryHit& hit)
		{
			PX_UNUSED(filterData0);
			PX_UNUSED(filterData1);
			PX_UNUSED(constantBlock);
			PX_UNUSED(constantBlockSize);
			if ((static_cast<const physx::PxSweepHit&>(hit)).hadInitialOverlap())
				return physx::PxQueryHitType::eNONE;
			return physx::PxQueryHitType::eTOUCH;
		}

		VehicleSceneQueryData::VehicleSceneQueryData()
			: mNumQueriesPerBatch(0),
			mNumHitResultsPerQuery(0),
			mRaycastResults(NULL),
			mRaycastHitBuffer(NULL),
			mPreFilterShader(NULL),
			mPostFilterShader(NULL)
		{
		}

		VehicleSceneQueryData::~VehicleSceneQueryData()
		{
		}





		/**
		* Need to make use of the fmemory
		*
		*/

		VehicleSceneQueryData* VehicleSceneQueryData::allocate
		(const uint32_t maxNumVehicles, const uint32_t maxNumWheelsPerVehicle, const uint32_t maxNumHitPointsPerWheel, const uint32_t numVehiclesInBatch,
			physx::PxBatchQueryPreFilterShader preFilterShader, physx::PxBatchQueryPostFilterShader postFilterShader,
			physx::PxAllocatorCallback& allocator)
		{
			const uint32_t sqDataSize = ((sizeof(VehicleSceneQueryData) + 15) & ~15);

			const uint32_t maxNumWheels = maxNumVehicles * maxNumWheelsPerVehicle;
			const uint32_t raycastResultSize = ((sizeof(physx::PxRaycastQueryResult) * maxNumWheels + 15) & ~15);
			const uint32_t sweepResultSize = ((sizeof(physx::PxSweepQueryResult) * maxNumWheels + 15) & ~15);

			const uint32_t maxNumHitPoints = maxNumWheels * maxNumHitPointsPerWheel;
			const uint32_t raycastHitSize = ((sizeof(physx::PxRaycastHit) * maxNumHitPoints + 15) & ~15);
			const uint32_t sweepHitSize = ((sizeof(physx::PxSweepHit) * maxNumHitPoints + 15) & ~15);

			const uint32_t size = sqDataSize + raycastResultSize + raycastHitSize + sweepResultSize + sweepHitSize;
			physx::PxU8* buffer = static_cast<physx::PxU8*>(allocator.allocate(size, NULL, NULL, 0));

			VehicleSceneQueryData* sqData = new(buffer) VehicleSceneQueryData();
			sqData->mNumQueriesPerBatch = numVehiclesInBatch * maxNumWheelsPerVehicle;
			sqData->mNumHitResultsPerQuery = maxNumHitPointsPerWheel;
			buffer += sqDataSize;

			sqData->mRaycastResults = reinterpret_cast<physx::PxRaycastQueryResult*>(buffer);
			buffer += raycastResultSize;

			sqData->mRaycastHitBuffer = reinterpret_cast<physx::PxRaycastHit*>(buffer);
			buffer += raycastHitSize;

			sqData->mSweepResults = reinterpret_cast<physx::PxSweepQueryResult*>(buffer);
			buffer += sweepResultSize;

			sqData->mSweepHitBuffer = reinterpret_cast<physx::PxSweepHit*>(buffer);
			buffer += sweepHitSize;

			for (uint32_t i = 0; i < maxNumWheels; i++)
			{
				new(sqData->mRaycastResults + i) physx::PxRaycastQueryResult();
				new(sqData->mSweepResults + i) physx::PxSweepQueryResult();
			}

			for (uint32_t i = 0; i < maxNumHitPoints; i++)
			{
				new(sqData->mRaycastHitBuffer + i) physx::PxRaycastHit();
				new(sqData->mSweepHitBuffer + i) physx::PxSweepHit();
			}

			sqData->mPreFilterShader = preFilterShader;
			sqData->mPostFilterShader = postFilterShader;

			return sqData;
		}

		void VehicleSceneQueryData::free(physx::PxAllocatorCallback& allocator)
		{
			allocator.deallocate(this);
		}

		physx::PxBatchQuery* VehicleSceneQueryData::setUpBatchedSceneQuery(const uint32_t batchId, const VehicleSceneQueryData& vehicleSceneQueryData, physx::PxScene* scene)
		{
			const uint32_t maxNumQueriesInBatch = vehicleSceneQueryData.mNumQueriesPerBatch;
			const uint32_t maxNumHitResultsInBatch = vehicleSceneQueryData.mNumQueriesPerBatch * vehicleSceneQueryData.mNumHitResultsPerQuery;

			physx::PxBatchQueryDesc sqDesc(maxNumQueriesInBatch, maxNumQueriesInBatch, 0);

			sqDesc.queryMemory.userRaycastResultBuffer = vehicleSceneQueryData.mRaycastResults + batchId * maxNumQueriesInBatch;
			sqDesc.queryMemory.userRaycastTouchBuffer = vehicleSceneQueryData.mRaycastHitBuffer + batchId * maxNumHitResultsInBatch;
			sqDesc.queryMemory.raycastTouchBufferSize = maxNumHitResultsInBatch;

			sqDesc.queryMemory.userSweepResultBuffer = vehicleSceneQueryData.mSweepResults + batchId * maxNumQueriesInBatch;
			sqDesc.queryMemory.userSweepTouchBuffer = vehicleSceneQueryData.mSweepHitBuffer + batchId * maxNumHitResultsInBatch;
			sqDesc.queryMemory.sweepTouchBufferSize = maxNumHitResultsInBatch;

			sqDesc.preFilterShader = vehicleSceneQueryData.mPreFilterShader;

			sqDesc.postFilterShader = vehicleSceneQueryData.mPostFilterShader;

			return scene->createBatchQuery(sqDesc);
		}

		physx::PxRaycastQueryResult* VehicleSceneQueryData::getRaycastQueryResultBuffer(const uint32_t batchId)
		{
			return (mRaycastResults + batchId * mNumQueriesPerBatch);
		}

		physx::PxSweepQueryResult* VehicleSceneQueryData::getSweepQueryResultBuffer(const uint32_t batchId)
		{
			return (mSweepResults + batchId * mNumQueriesPerBatch);
		}


		uint32_t VehicleSceneQueryData::getQueryResultBufferSize() const
		{
			return mNumQueriesPerBatch;
		}
	}
}