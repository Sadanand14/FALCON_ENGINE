#ifndef VEHICLE_H
#define VEHICLE_H

#include "PxPhysicsAPI.h"
#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
#include "../Physics.h"
#include "Memory/fnew.h"

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

			float gVehicleModeTimer;
			float gVehicleModeLifetime;
			int gVehicleOrderProgress;
			bool gVehicleOrderComplete;
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
		* Car creation/Deletion API
		*/
		void CreateCar(physx::PxRigidDynamic* vehActor, Transform& startTransform);
		void ReleaseCarMemory();

		/**
		* Vehicle Update
		*/

		void StepVehicleSDK(float dt);

		//Creating friction pairs for simulation calculations.
		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* createFrictionPairs(const physx::PxMaterial* defaultMaterial);




		/**
		* Car data structures for holding car data.
		*/

		struct ActorUserData
		{
			ActorUserData()
				: vehicle(NULL),
				actor(NULL)
			{
			}

			const physx::PxVehicleWheels* vehicle;
			const physx::PxActor* actor;
		};

		struct ShapeUserData
		{
			ShapeUserData()
				: isWheel(false),
				wheelId(0xffffffff)
			{
			}

			bool isWheel;
			uint32_t wheelId;
		};


		struct VehicleDesc
		{
			float chassisMass;
			physx::PxVec3 chassisDims;
			physx::PxVec3 chassisMOI;
			physx::PxVec3 chassisCMOffset;
			physx::PxMaterial* chassisMaterial;
			physx::PxFilterData chassisSimFilterData;  //word0 = collide type, word1 = collide against types, word2 = physx::PxPairFlags

			float wheelMass;
			float wheelWidth;
			float wheelRadius;
			float wheelMOI;
			physx::PxMaterial* wheelMaterial;
			float numWheels;
			physx::PxFilterData wheelSimFilterData;	//word0 = collide type, word1 = collide against types, word2 = physx::PxPairFlags

			ActorUserData* actorUserData;
			ShapeUserData* shapeUserDatas;


			VehicleDesc()
				: chassisMass(0.0f),
				chassisDims(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisMOI(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisCMOffset(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				chassisMaterial(NULL),
				wheelMass(0.0f),
				wheelWidth(0.0f),
				wheelRadius(0.0f),
				wheelMOI(0.0f),
				wheelMaterial(NULL),
				actorUserData(NULL),
				shapeUserDatas(NULL)
			{
			}
		};


		struct Car
		{

			VehicleDesc m_carDesc;
			physx::PxVehicleDrive4W* m_car;
			bool m_isInAir;
			physx::PxVehicleDrive4WRawInputData m_vehicleInputData;

			Car(physx::PxRigidDynamic* vehActor, Transform& startTransform);

			~Car()
			{
				//PX_RELEASE(m_car);
				//m_car->free();
			}
		};
		
		/**
		* Global vec to maintain all cars' Driver4W component. This is used by vehicle update to update all the cars.
		* As custom allocators are not initialized before these allocations, I am using default allocators.
		*/

		extern std::vector<Car*>gCars;
		extern bool gMimicKeyInputs;

	}


}

#endif //VEHICLE_H