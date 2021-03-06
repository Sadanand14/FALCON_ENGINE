#ifndef VEHICLE_H
#define VEHICLE_H

#include "Types.h"
#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
#include "../Physics.h"
#include "Memory/fnew.h"
#include "VehicleInputHandler.h"


namespace physics
{

	namespace vehicle
	{


		/*
		* A nameless namespace to create private global variables.
		*/

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
					0.060f,	//rise rate eANALOG_INPUT_ACCEL
					0.10f,	//rise rate eANALOG_INPUT_BRAKE		
					0.10f,	//rise rate eANALOG_INPUT_HANDBRAKE	
					0.025f,	//rise rate eANALOG_INPUT_STEER_LEFT
					0.025f,	//rise rate eANALOG_INPUT_STEER_RIGHT
				},
				{
					0.0100f,	//fall rate eANALOG_INPUT_ACCEL
					0.100f,	//fall rate eANALOG_INPUT_BRAKE		
					0.100f,	//fall rate eANALOG_INPUT_HANDBRAKE	
					0.050f,	//fall rate eANALOG_INPUT_STEER_LEFT
					0.050f	   //fall rate eANALOG_INPUT_STEER_RIGHT
				}
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
		* Predefined driving modes.
		*/
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
		bool MakeActorDrivableSurface(physx::PxShape* shape);



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

		/**
		* A data structure to hold the vehicle physics properties. Which will be used by
		* physx to simulate behavior.
		*/
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



		/**
		* Car data structure is used to create a vehicle on which we will apply physx simulations.
		*/
		struct Car
		{

			VehicleDesc m_carDesc;
			physx::PxVehicleDrive4W* m_car;
			bool m_isInAir;

			void CustomizeCarToEngineScale(float scale);
			//FLVehicleDrive4WRawInputData m_vehicleInputData;

			Car(physx::PxRigidDynamic* vehActor, Transform& startTransform);

			~Car() = default;
		};
		


		/**
		* Controller class to handle car which exposes movement API for car motions. 
		*/
		class CarController
		{
		public:
			CarController(bool mimicInput = false);
			~CarController() = default;
			void SetDriveMode(DriveMode drivemode, Car* car, physx::PxVehicleDrive4WRawInputData& vehicleInputData);
			
			

			inline bool ShouldMimicInput() const { return mMimicInput; }
		private:
			bool mMimicInput;
		};

		/**
		* Global vec to maintain all cars' Driver4W component. This is used by vehicle update to update all the cars.
		* As custom allocators are not initialized before these allocations, I am using default allocators.
		*/

		extern boost::container::vector<Car*>gCars;
		extern boost::container::vector<CarController*>gCarControllers;
		extern boost::container::vector<physx::PxVehicleWheels*>gVehicles;
		extern physx::PxVehicleDrive4WRawInputData gVehicleInputData[10];
		extern boost::container::vector<physx::PxVehicleWheelQueryResult>gvehicleQueryResults;
		extern bool gMimicKeyInputs;
		extern bool gIsVehicleInScene;
		extern bool gIsInputDigital;
		/**
		* Car API
		*/
		Car* CreateCar(physx::PxRigidDynamic* vehActor, Transform& startTransform);
		void ReleaseCarMemory();
		
		CarController* GetCarContoller(const Car* car);
		void ApplyInputToCar(Car* car, DriveMode& driveMode);
	}


}

#endif //VEHICLE_H