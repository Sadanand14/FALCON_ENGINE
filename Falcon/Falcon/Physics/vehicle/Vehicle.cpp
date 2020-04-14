#include "Physx/physx/include/vehicle/PxVehicleUtil.h"
#include "Vehicle.h"
#include "VehicleSceneQuery.h"
#include "VehicleFilterShader.h"
#include "WheelQueryResult.h"
#include "CarAPI.h"
#include "../PXMathUtils.h"
namespace physics
{

	namespace vehicle
	{
		/**
		* Global vec to maintain all cars' Driver4W component. This is used by vehicle update to update all the cars.
		* As custom allocators are not initialized before these allocations, I am using default allocators.
		*/
		boost::container::vector<Car*>gCars;
		boost::container::vector<CarController*>gCarControllers;
		physx::PxVehicleDrive4WRawInputData gVehicleInputData[10];
		boost::container::vector<physx::PxVehicleWheels*>gVehicles;
		boost::container::vector<physx::PxVehicleWheelQueryResult> gvehicleQueryResults;
		bool gMimicKeyInputs = false;
		bool gIsVehicleInScene = false;
		namespace
		{
			//physx::PxMaterial * gTarmacMaterial = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);;
			VehicleSceneQueryData* gVehicleSceneQueryData = NULL;
			physx::PxBatchQuery* gBatchQuery = NULL;
			physx::PxVehicleDrivableSurfaceToTireFrictionPairs* gFrictionPairs = NULL;
			physx::PxRigidStatic* gGroundPlane = NULL;
			std::unordered_map<Car*, CarController*> gCarControllerMap;
			std::unordered_map<Car*, int> gCarIndexMap;

			/**
			* Helper method to create a drivable plane.
			* @param simulation filter data
			* @param material for plane
			* @param physics instance in the scene
			* @return Rigidstatic* to the plane
			*/
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




		/**
		* Initializes the physx vehicle sdk.
		* @return true if success,false otherwise.
		*/
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

				//gCars.resize(1);

				gVehicleModeTimer = 0.0f;
				gVehicleModeLifetime = 5.0f;
				gVehicleOrderProgress = 0;

				FL_ENGINE_INFO("INFO: Vehcile SDK initiated successfully.");
				return true;
			}
			catch (std::exception& e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to init the vehicle sdk. {0}", e.what());
				return false;
			}
		}


		/**
		* Releases resources being held physx vehicle sdk.
		* @return true if success,false otherwise.
		*/
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
				ReleaseCarMemory();
				return true;
			}
			catch (std::exception& e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to release the vehicle sdk. {0}", e.what());
				return false;
			}

		}


		/**
		* Handles simulation setup for the vehicles in the scene.
		* @param time delta
		*/

		void StepVehicleSDK(float dt)
		{

			try
			{
				//Cycle through the driving modes to demonstrate how to accelerate/reverse/brake/turn etc.

				/*for (uint8_t itr = 0; itr < gCars.size() && gCars[itr] != nullptr; ++itr)
				{
					IncrementDrivingMode(gCars[itr], gVehicleInputData[itr], dt);
				}*/

				//Raycasts
				//if (gVehicles[0] != nullptr)
				{
					physx::PxRaycastQueryResult* raycastResults = gVehicleSceneQueryData->getRaycastQueryResultBuffer(0);
					const uint32_t raycastResultsSize = gVehicleSceneQueryData->getQueryResultBufferSize();
					PxVehicleSuspensionRaycasts(gBatchQuery, gCars.size(), &gVehicles[0], raycastResultsSize, raycastResults);

					//FL_ENGINE_WARN("RAYCASTS DONE");
				}

				//vehicle update
				physx::PxWheelQueryResult wheelQueryResults[4];
				physx::PxVec3 grav = GetPhysicsScene()->getGravity();
				if (gvehicleQueryResults.size() < gVehicles.size())
					gvehicleQueryResults.resize(gVehicles.size());
				for (unsigned itr = 0; itr < gVehicles.size(); ++itr)
				{
					//PxVehicleWheelQueryResult vehicleQueryResults[1] = { {wheelQueryResults, gVehicle4W->mWheelsSimData.getNbWheels()} };
					physx::PxVehicleWheelQueryResult temp{ wheelQueryResults ,gVehicles[itr]->mWheelsSimData.getNbWheels() };
					gvehicleQueryResults[itr] = temp;
					//Update the control inputs for the vehicle.
					if (gMimicKeyInputs)
					{
						PxVehicleDrive4WSmoothDigitalRawInputsAndSetAnalogInputs(gKeySmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData[0], dt, gCars[itr]->m_isInAir, *gCars[itr]->m_car);
					}
					else
					{
						PxVehicleDrive4WSmoothAnalogRawInputsAndSetAnalogInputs(gPadSmoothingData, gSteerVsForwardSpeedTable, gVehicleInputData[0], dt, gCars[itr]->m_isInAir, *gCars[itr]->m_car);
					}
					//FL_ENGINE_WARN("INPUTS DONE");
				}

				//if (gVehicles[0] != nullptr)
				{
					physx::PxVehicleUpdates(dt, grav, *gFrictionPairs, gVehicles.size(), &gVehicles[0], &gvehicleQueryResults[0]);
					//FL_ENGINE_WARN("UPDATES DONE");
				}


				//Needed for movement handling
				for (uint8_t itr = 0; itr < gCars.size(); ++itr)
				{
					gCars[itr]->m_isInAir = gCars[itr]->m_car->getRigidDynamicActor()->isSleeping() ? false : physx::PxVehicleIsInAir(gvehicleQueryResults[itr]);
				}
			}
			catch (std::exception& e)
			{
				FL_ENGINE_ERROR("ERROR: Vehicle update failed, {0}", e.what());
			}

		}




		/**
		* Creates the firction pairs for the tyre materials and terrain surfaces. Will be used by the simulation.
		* @param material pointer for default material
		* @return pointer to data structure holding Surface to tire friction pairs.
		*/
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



		/**
		* Car structure constructor
		* @param Rigiddynamic* to the actor
		* @param starting transform for the car
		*/
		Car::Car(physx::PxRigidDynamic* vehActor, Transform& startTransform)
			: m_car(nullptr),
			m_isInAir(true)
		{
			CreateVehicleDescriptionObject(this);
			CreateVehicle4W(this, vehActor);
			m_car->getRigidDynamicActor()->setGlobalPose(physx::PxTransform(*PXMathUtils::Vec3ToPxVec3(startTransform.GetPosition()),
				*PXMathUtils::QuatToPxQuat(startTransform.GetRotation())));
		}





		CarController::CarController(bool mimicInput /*=false*/)
			:mMimicInput(mimicInput)
		{
			//Making car stay still at the begining
			//ReleaseAllControls(car, vehicleInputData);
		}

		void CarController::SetDriveMode(DriveMode drivemode, Car* car, physx::PxVehicleDrive4WRawInputData& vehicleInputData)
		{
			ReleaseAllControls(car, vehicleInputData);

			//Start driving in the selected mode.
			//DriveMode eDriveMode = gDriveModeOrder[gVehicleOrderProgress];
			switch (drivemode)
			{
			case eDRIVE_MODE_ACCEL_FORWARDS:
				StartAccelerateForwardsMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_ACCEL_REVERSE:
				StartAccelerateReverseMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_HARD_TURN_LEFT:
				StartTurnHardLeftMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_HANDBRAKE_TURN_LEFT:
				StartHandbrakeTurnLeftMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_HARD_TURN_RIGHT:
				StartTurnHardRightMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_HANDBRAKE_TURN_RIGHT:
				StartHandbrakeTurnRightMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_BRAKE:
				StartBrakeMode(car, vehicleInputData);
				break;
			case eDRIVE_MODE_NONE:
				break;
			};

			//If the mode about to Start is eDRIVE_MODE_ACCEL_REVERSE then switch to reverse gears.
			/*if (eDRIVE_MODE_ACCEL_REVERSE == gDriveModeOrder[gVehicleOrderProgress])
			{
				car->m_car->mDriveDynData.forceGearChange(physx::PxVehicleGearsData::eREVERSE);
			}*/
		}









		/**
		* Car Creation API
		* @param Rigiddynamic* to the actor
		* @param starting transform for the car
		*/
		Car* CreateCar(physx::PxRigidDynamic* vehActor, Transform& transform)
		{
			try
			{
				Car* car = fmemory::fnew<Car>(vehActor, transform);
				//Register car to the global car data
				gCars.push_back(car);
				gVehicles.push_back(car->m_car);
				//Create a car controller
				CarController* controller = fmemory::fnew<CarController>(false);
				gCarControllers.push_back(controller);
				gCarControllerMap[car] = controller;

				gCarIndexMap[car] = gCars.size() - 1;


				gIsVehicleInScene =  true;
				return car;
			}
			catch (std::exception &e )
			{
				FL_ENGINE_ERROR("ERROR: Failed to create car. {0}", e.what());
				return nullptr;
			}
		}

		/**
		* Releases the memory being used by the car objects.
		*/
		void ReleaseCarMemory()
		{
			for (Car* car : gCars)
			{
				if (car != nullptr)
					fmemory::fdelete<Car>(car);
			}

			for (CarController* controller : gCarControllers)
			{
				if (controller != nullptr)
					fmemory::fdelete<CarController>(controller);
			}
		}


		/**
		* Get the car controller associated with the car* provided.
		* @param car pointer to the car
		* @return controller assoiciated with the car* if exists or nullptr
		*/
		CarController* GetCarContoller(Car* car)
		{
			if (gCarControllerMap.find(car) != gCarControllerMap.end())
				return gCarControllerMap[car];
			else
			{
				FL_ENGINE_ERROR("Failed to find controller for the car");
				return nullptr;
			}
		}



		void ApplyInputToCar(Car* car, DriveMode& driveMode)
		{
			try
			{
				int carIndex = gCarIndexMap[car];
				CarController ref = gCarControllers[carIndex];
				physx::PxVehicleDrive4WRawInputData inputData = gVehicleInputData[carIndex];
				ref.SetDriveMode(driveMode,car, gVehicleInputData[carIndex]);
				
			}
			catch (std::exception& e)
			{
				FL_ENGINE_ERROR("ERROR: Failed to apply input to car {0}", e.what());
				
			}
			return;
		}


	}
}