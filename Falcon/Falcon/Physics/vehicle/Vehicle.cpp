#include "Vehicle.h"
#include "VehicleSceneQuery.h"
#include "VehicleFilterShader.h"
#include "WheelQueryResult.h"
namespace physics
{

	namespace vehicle
	{
		namespace 
		{
			//physx::PxMaterial * gTarmacMaterial = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);;
			VehicleSceneQueryData* gVehicleSceneQueryData = NULL;
			physx::PxBatchQuery* gBatchQuery = NULL;
			physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
		}

		bool InitVehicleSDK()
		{
			try
			{
				physx::PxPhysics* physics = GetPhysics();
				physx::PxInitVehicleSDK(*physics);
				physx::PxVehicleSetBasisVectors(physx::PxVec3(0, 1, 0), physx::PxVec3(0, 0, 1));
				physx::PxVehicleSetUpdateMode(physx::PxVehicleUpdateMode::eVELOCITY_CHANGE);

				/* Next steps
					1. Create batched scene querries for suspension
					2. Create Friction table
					3. Create a surface to drive on? Unknown: how terrain will work ?
					4. Create Vehicle - Done by car API

					Additional support needed for: 
					1. Vehicle Batched querries
					2. Vehicle creation
					3. Tires and materials API
				*/

				//Create the batched scene queries for the suspension raycasts. May be this should be in CAR API.
				physx::PxDefaultAllocator allocator = GetAllocator();
				gVehicleSceneQueryData = VehicleSceneQueryData::allocate(1, PX_MAX_NB_WHEELS, 1, 1, WheelSceneQueryPreFilterBlocking, NULL, allocator);
				gBatchQuery = VehicleSceneQueryData::setUpBatchedSceneQuery(0, *gVehicleSceneQueryData, GetPhysicsScene());

				//Create the friction table for each combination of tire and surface type.
				gFrictionPairs = createFrictionPairs(GetDefaultMaterial() /*Kind of default material*/);

				FL_ENGINE_INFO("INFO: Vehcile SDK initiated successfully.");
				return true;
			}
			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to init the vehicle sdk. {0}", e.what());
				return false;
			}
		}
		bool ReleaseVehcileSDK()
		{
			try
			{
				//PX_RELEASE(gGroundPlane);
				PX_RELEASE(gBatchQuery);
				physx::PxDefaultAllocator allocator = GetAllocator();
				gVehicleSceneQueryData->free(allocator);
				PX_RELEASE(gFrictionPairs);
				physx::PxCloseVehicleSDK();
			}
			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to release the vehicle sdk. {0}", e.what());
				return false;
			}
			
		}


		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* createFrictionPairs(const physx::PxMaterial* defaultMaterial)
		{
			physx::PxVehicleDrivableSurfaceType surfaceTypes[1];
			surfaceTypes[0].mType = SURFACE_TYPE_TARMAC;

			const physx::PxMaterial* surfaceMaterials[1];
			surfaceMaterials[0] = defaultMaterial;

			physx::PxVehicleDrivableSurfaceToTireFrictionPairs* surfaceTirePairs =
				physx::PxVehicleDrivableSurfaceToTireFrictionPairs::allocate(MAX_NUM_TIRE_TYPES, MAX_NUM_SURFACE_TYPES);

			surfaceTirePairs->setup(MAX_NUM_TIRE_TYPES, MAX_NUM_SURFACE_TYPES, surfaceMaterials, surfaceTypes);

			for (size_t surfaceItr = 0; surfaceItr < MAX_NUM_SURFACE_TYPES; surfaceItr++)
			{
				for (size_t tyreItr = 0; tyreItr < MAX_NUM_TIRE_TYPES; tyreItr++)
				{
					surfaceTirePairs->setTypePairFriction(surfaceItr, tyreItr, gTireFrictionMultipliers[surfaceItr][tyreItr]);
				}
			}
			return surfaceTirePairs;
		}
	}
}