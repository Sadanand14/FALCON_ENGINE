#ifndef WHEELQUERYRESULT_H
#define WHEELQUERYRESULT_H

namespace physics
{
	namespace vehicle
	{
		//Data structure for quick setup of wheel query data structures.
		class WheelQueryResults
		{
		public:

			WheelQueryResults()
				: mWheelQueryResults(NULL)
			{
			}

			~WheelQueryResults()
			{
			}

			//Allocate wheel results for up to maxNumVehicles with up to maxNumWheelsPerVehicle.
			static WheelQueryResults* allocate(const uint32_t maxNumVehicles, const uint32_t maxNumWheelsPerVehicle, physx::PxAllocatorCallback& allocator)
			{
				const uint32_t byteSize = sizeof(WheelQueryResults) + sizeof(physx::PxVehicleWheelQueryResult) * maxNumVehicles + sizeof(physx::PxWheelQueryResult) * maxNumWheelsPerVehicle * maxNumVehicles;

				physx::PxU8* buffer = static_cast<physx::PxU8*>(allocator.allocate(byteSize, NULL, NULL, 0));

				WheelQueryResults* vwqr = reinterpret_cast<WheelQueryResults*>(buffer);
				buffer += sizeof(WheelQueryResults);

				vwqr->mWheelQueryResults = reinterpret_cast<physx::PxVehicleWheelQueryResult*>(buffer);
				buffer += sizeof(physx::PxVehicleWheelQueryResult) * maxNumVehicles;

				for (uint32_t i = 0; i < maxNumVehicles; i++)
				{
					new(buffer) physx::PxWheelQueryResult();
					vwqr->mWheelQueryResults[i].wheelQueryResults = reinterpret_cast<physx::PxWheelQueryResult*>(buffer);
					vwqr->mWheelQueryResults[i].nbWheelQueryResults = maxNumWheelsPerVehicle;
					buffer += sizeof(physx::PxWheelQueryResult) * maxNumWheelsPerVehicle;
				}

				return vwqr;
			}

			//Free allocated buffer for scene queries of suspension raycasts.
			void free(physx::PxAllocatorCallback& allocator)
			{
				allocator.deallocate(this);
			}

			//Return the PxVehicleWheelQueryResult for a vehicle specified by an index.
			physx::PxVehicleWheelQueryResult* getWheelQueryResults(const uint32_t id)
			{
				return (mWheelQueryResults + id);
			}

		private:

			physx::PxVehicleWheelQueryResult* mWheelQueryResults;
		};
	}
}

#endif // !WHEELQUERYRESULT_H
