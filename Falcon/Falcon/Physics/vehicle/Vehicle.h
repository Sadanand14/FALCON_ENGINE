#ifndef VEHICLE_H
#define VEHICLE_H

#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
#include "../physics.h"
#include "Memory/fnew.h"
#include "Car.h"
namespace physics
{

	namespace vehicle
	{

		/**
		* Global vec to maintain all cars.
		*/

		std::vector<vehicle::Car*, fmemory::STLAllocator<Car*>>gAllCars;


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
		}




		/**
		* Initiates the vehicle sdk for the physics.
		**/
		bool InitVehicleSDK();

		/**
		* Releases resources allocated by the vehicle sdk.
		*/
		bool ReleaseVehcileSDK();



		/**
		* Vehicle Update
		*/

		void StepVehicleSDK(float dt);

		//Creating friction pairs for simulation calculations.
		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* createFrictionPairs(const physx::PxMaterial* defaultMaterial);
		


	}


}

#endif //VEHICLE_H