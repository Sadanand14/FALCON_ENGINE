#include "Vehicle.h"

namespace physics
{

	namespace vehicle
	{
		namespace
		{
			physx::PxF32 gSteerVsForwardSpeedData[2 * 8] =
			{
				0.0f,		0.75f,
				5.0f,		0.75f,
				30.0f,		0.125f,
				120.0f,		0.1f,
				PX_MAX_F32, PX_MAX_F32,
				PX_MAX_F32, PX_MAX_F32,
				PX_MAX_F32, PX_MAX_F32,
				PX_MAX_F32, PX_MAX_F32
			};
			physx::PxFixedSizeLookupTable<8> gSteerVsForwardSpeedTable(gSteerVsForwardSpeedData, 4);

			physx::PxVehicleKeySmoothingData gKeySmoothingData =
			{
				{
					6.0f,	//rise rate eANALOG_INPUT_ACCEL
					6.0f,	//rise rate eANALOG_INPUT_BRAKE		
					6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
					2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
					2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
				},
				{
					10.0f,	//fall rate eANALOG_INPUT_ACCEL
					10.0f,	//fall rate eANALOG_INPUT_BRAKE		
					10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
					5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
					5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
				}
			};

			physx::PxVehiclePadSmoothingData gPadSmoothingData =
			{
				{
					6.0f,	//rise rate eANALOG_INPUT_ACCEL
					6.0f,	//rise rate eANALOG_INPUT_BRAKE		
					6.0f,	//rise rate eANALOG_INPUT_HANDBRAKE	
					2.5f,	//rise rate eANALOG_INPUT_STEER_LEFT
					2.5f,	//rise rate eANALOG_INPUT_STEER_RIGHT
				},
				{
					10.0f,	//fall rate eANALOG_INPUT_ACCEL
					10.0f,	//fall rate eANALOG_INPUT_BRAKE		
					10.0f,	//fall rate eANALOG_INPUT_HANDBRAKE	
					5.0f,	//fall rate eANALOG_INPUT_STEER_LEFT
					5.0f	//fall rate eANALOG_INPUT_STEER_RIGHT
				}
			};

			physx::PxVehicleDrive4WRawInputData gVehicleInputData;

			enum DriveMode
			{
				eDRIVE_MODE_ACCEL_FORWARDS = 0,
				eDRIVE_MODE_ACCEL_REVERSE,
				eDRIVE_MODE_HARD_TURN_LEFT,
				eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
				eDRIVE_MODE_HARD_TURN_RIGHT,
				eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
				eDRIVE_MODE_BRAKE,
				eDRIVE_MODE_NONE
			};

			DriveMode gDriveModeOrder[] =
			{
				eDRIVE_MODE_BRAKE,
				eDRIVE_MODE_ACCEL_FORWARDS,
				eDRIVE_MODE_BRAKE,
				eDRIVE_MODE_ACCEL_REVERSE,
				eDRIVE_MODE_BRAKE,
				eDRIVE_MODE_HARD_TURN_LEFT,
				eDRIVE_MODE_BRAKE,
				eDRIVE_MODE_HARD_TURN_RIGHT,
				eDRIVE_MODE_ACCEL_FORWARDS,
				eDRIVE_MODE_HANDBRAKE_TURN_LEFT,
				eDRIVE_MODE_ACCEL_FORWARDS,
				eDRIVE_MODE_HANDBRAKE_TURN_RIGHT,
				eDRIVE_MODE_NONE
			};

			//Drivable surface types. Later on can be updated via json or if less count can be done manually.
			enum SurfaceTypes 
			{
				SURFACE_TYPE_TARMAC,
				MAX_NUM_SURFACE_TYPES
			};

			//Tire types.
			enum
			{
				TIRE_TYPE_NORMAL = 0,
				TIRE_TYPE_WORN,
				MAX_NUM_TIRE_TYPES
			};

			//Tire model friction for each combination of drivable surface type and tire type.
			static float gTireFrictionMultipliers[MAX_NUM_SURFACE_TYPES][MAX_NUM_TIRE_TYPES] =
			{
				//NORMAL,	WORN
				{1.00f,		0.1f}//TARMAC
			};

			physx::PxMaterial * gTarmacMaterial = GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);;

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
					4. Create Vehicle

					Additional support needed for: 
					1. Vehicle Batched querries
					2. Vehicle creation
					3. Tires and materials API
				*/
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
				//PX_RELEASE(gBatchQuery);
				//physx::gVehicleSceneQueryData->free(gAllocator);
				//PX_RELEASE(gFrictionPairs);
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