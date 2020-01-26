#ifndef VEHICLE_H
#define VEHICLE_H

#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
#include "../physics.h"
namespace physics
{

	namespace vehicle
	{
		bool InitVehicleSDK();
		bool ReleaseVehcileSDK();

		//Creating friction pairs for simulation calculations.
		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* createFrictionPairs(const physx::PxMaterial* defaultMaterial);
		//May be a class to give an API for vehicle creation
	}


}

#endif //VEHICLE_H