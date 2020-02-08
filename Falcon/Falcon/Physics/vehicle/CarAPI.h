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
		void IncrementDrivingMode(Car* car, const float timestep);

		/**
		* Methods to pass on inuts to phyX.
		*/
		void startAccelerateForwardsMode(Car* car);
		void startAccelerateReverseMode(Car* car);
		void startBrakeMode(Car* car);
		void startTurnHardLeftMode(Car* car);
		void startTurnHardRightMode(Car* car);
		void startHandbrakeTurnLeftMode(Car* car);
		void startHandbrakeTurnRightMode(Car* car);
		void releaseAllControls(Car* car);
	}
}

#endif // !CAR_H
