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
		void CustomizeVehicleToLengthScale(const physx::PxReal lengthScale, physx::PxRigidDynamic* rigidDynamic, physx::PxVehicleWheelsSimData* wheelsSimData, physx::PxVehicleDriveSimData* driveSimData);
		void ScaleAckermanData(float lengthScale, physx::PxVehicleDriveSimData4W* driveSimData);
		/**
		* Methods to pass on inuts to phyX.
		*/
		void StartAccelerateForwardsMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartAccelerateReverseMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartBrakeMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartTurnHardLeftMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartTurnHardRightMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartHandbrakeTurnLeftMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void StartHandbrakeTurnRightMode(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
		void ReleaseAllControls(Car* car, physx::PxVehicleDrive4WRawInputData& gVehicleInputData);
	}
}

#endif // !CAR_H
