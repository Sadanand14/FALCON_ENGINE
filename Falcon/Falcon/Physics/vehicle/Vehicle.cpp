#include "Physx/physx/include/vehicle/PxVehicleUtil.h"
#include "Vehicle.h"
#include "VehicleSceneQuery.h"
#include "VehicleFilterShader.h"
#include "WheelQueryResult.h"
#include "Car.h"
namespace physics
{

	namespace vehicle
	{

		std::vector<physx::PxVehicleWheels*, fmemory::STLAllocator<physx::PxVehicleWheels*>>gVehicles;
		std::vector<bool, fmemory::STLAllocator<bool>>gIsVehicleInAir;
		namespace 
		{
			//physx::PxMaterial * gTarmacMaterial = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);;
			VehicleSceneQueryData* gVehicleSceneQueryData = NULL;
			physx::PxBatchQuery* gBatchQuery = NULL;
			physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
			physx::PxRigidStatic* gGroundPlane = NULL;


			physx::PxRigidStatic* createDrivablePlane(const physx::PxFilterData& simFilterData, physx::PxMaterial* material, physx::PxPhysics* physics)
			{
				//Add a plane to the scene.
				physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 0), *material);

				//Get the plane shape so we can set query and simulation filter data.
				physx::PxShape* shapes[1];
				groundPlane->getShapes(shapes, 1);

				//Set the query filter data of the ground plane so that the vehicle raycasts can hit the ground.
				physx::PxFilterData qryFilterData;
				setupDrivableSurface(qryFilterData);
				shapes[0]->setQueryFilterData(qryFilterData);

				//Set the simulation filter data of the ground plane so that it collides with the chassis of a vehicle but not the wheels.
				shapes[0]->setSimulationFilterData(simFilterData);

				return groundPlane;
			}
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

				physx::PxFilterData groundPlaneSimFilterData(COLLISION_FLAG_GROUND, COLLISION_FLAG_GROUND_AGAINST, 0, 0);
				gGroundPlane = createDrivablePlane(groundPlaneSimFilterData, GetDefaultMaterial(), GetPhysics());
				GetPhysicsScene()->addActor(*gGroundPlane);

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
				PX_RELEASE(gGroundPlane);
				PX_RELEASE(gBatchQuery);
				physx::PxDefaultAllocator allocator = GetAllocator();
				gVehicleSceneQueryData->free(allocator);
				PX_RELEASE(gFrictionPairs);
				physx::PxCloseVehicleSDK();
				return true;
			}
			catch (std::exception & e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to release the vehicle sdk. {0}", e.what());
				return false;
			}
			
		}




		void StepVehicleSDK(float dt)
		{
			
			/*gVehicles.resize(gAllCars.size());
			for (uint8_t itr = 0; itr < gAllCars.size(); ++itr)
			{
				gVehicles.emplace_back(gAllCars[itr]->GetDriveComponent());
			}*/

			//Raycasts
			physx::PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
			const uint32_t raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
			PxVehicleSuspensionRaycasts(gBatchQuery, gVehicles.size(), &gVehicles[0], raycastResultsSize, raycastResults);


			//vehicle update
			physx::PxWheelQueryResult wheelQueryResults[1][4];
			physx::PxVec3 grav = GetPhysicsScene()->getGravity();
			std::vector<physx::PxVehicleWheelQueryResult, fmemory::STLAllocator<physx::PxVehicleWheelQueryResult>>vehicleQueryResults;
			vehicleQueryResults.resize(gVehicles.size());
			for (uint8_t itr = 0; itr < gVehicles.size(); ++itr)
			{
				//PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
				physx::PxVehicleWheelQueryResult temp{ wheelQueryResults[itr] ,gVehicles[itr]->mWheelsSimData.getNbWheels() };
				vehicleQueryResults.emplace_back(temp);
			}

			physx::PxVehicleUpdates(dt, grav, *gFrictionPairs, gVehicles.size(), &gVehicles[0], &vehicleQueryResults[0]);

			//Needed for movement handling
			for (uint8_t itr = 0; itr < gVehicles.size(); ++itr)
			{
				gIsVehicleInAir[itr]= gVehicles[itr]->getRigidDynamicActor()->isSleeping() ? false : physx::PxVehicleIsInAir(vehicleQueryResults[itr]);
			}
		
			//Scene update
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