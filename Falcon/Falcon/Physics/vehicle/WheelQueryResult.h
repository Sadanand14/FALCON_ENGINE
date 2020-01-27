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
			static WheelQueryResults* allocate(const PxU32 maxNumVehicles, const PxU32 maxNumWheelsPerVehicle, PxAllocatorCallback& allocator)
			{
				const PxU32 byteSize = sizeof(WheelQueryResults) + sizeof(PxVehicleWheelQueryResult) * maxNumVehicles + sizeof(PxWheelQueryResult) * maxNumWheelsPerVehicle * maxNumVehicles;

				PxU8* buffer = static_cast<PxU8*>(allocator.allocate(byteSize, NULL, NULL, 0));

				WheelQueryResults* vwqr = reinterpret_cast<WheelQueryResults*>(buffer);
				buffer += sizeof(WheelQueryResults);

				vwqr->mWheelQueryResults = reinterpret_cast<PxVehicleWheelQueryResult*>(buffer);
				buffer += sizeof(PxVehicleWheelQueryResult) * maxNumVehicles;

				for (PxU32 i = 0; i < maxNumVehicles; i++)
				{
					new(buffer) PxWheelQueryResult();
					vwqr->mWheelQueryResults[i].wheelQueryResults = reinterpret_cast<PxWheelQueryResult*>(buffer);
					vwqr->mWheelQueryResults[i].nbWheelQueryResults = maxNumWheelsPerVehicle;
					buffer += sizeof(PxWheelQueryResult) * maxNumWheelsPerVehicle;
				}

				return vwqr;
			}

			//Free allocated buffer for scene queries of suspension raycasts.
			void free(PxAllocatorCallback& allocator)
			{
				allocator.deallocate(this);
			}

			//Return the PxVehicleWheelQueryResult for a vehicle specified by an index.
			PxVehicleWheelQueryResult* getWheelQueryResults(const PxU32 id)
			{
				return (mWheelQueryResults + id);
			}

		private:

			PxVehicleWheelQueryResult* mWheelQueryResults;
		};
	}
}

#endif // !WHEELQUERYRESULT_H
