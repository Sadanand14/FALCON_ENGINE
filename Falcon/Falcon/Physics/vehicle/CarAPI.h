#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
#include "VehicleFilterShader.h"
#include "VehicleSceneQuery.h"
#define WHEEL_COUNT 4

class Mesh;
struct Transform;

namespace physics
{
	namespace vehicle
	{
		/**
		* The API which exposes the vehicle creation for the external classes to create cars
		* to create and use cars in scenes.
		* Steps to instantiate a car 
		* 1. Setup wheel data
		* 2. Setup DriveSimulation data
		* 3. Adds cars to the 
		*/

		void CreateVehicleDescriptionObject(Car* car);
		void CreateVehicle4W(Car* car,physx::PxRigidDynamic* vehActor);
		void SetupVehicleActorData(Car* car,physx::PxRigidDynamic* vehActor);
		void ConfigureCarData(physx::PxVehicleWheels* vehicle, ActorUserData* actorUserData, ShapeUserData* shapeUserDatas);
		void IncrementDrivingMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData,const float timestep);

		/**
		* Methods to pass on inuts to phyX.
		*/
		void startAccelerateForwardsMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startAccelerateReverseMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startBrakeMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startTurnHardLeftMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startTurnHardRightMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startHandbrakeTurnLeftMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void startHandbrakeTurnRightMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void releaseAllControls(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
	}
}

#endif // !CAR_H
