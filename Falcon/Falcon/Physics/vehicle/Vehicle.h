#ifndef VEHICLE_H
#define VEHICLE_H

#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
#include "../physics.h"
namespace physics
{

	namespace vehicle
	{
		/**
		* Initiates the vehicle sdk for the physics.
		**/
		bool InitVehicleSDK();

		/**
		* Releases resources allocated by the vehicle sdk.
		*/
		bool ReleaseVehcileSDK();

		//Creating friction pairs for simulation calculations.
		physx::PxVehicleDrivableSurfaceToTireFrictionPairs* createFrictionPairs(const physx::PxMaterial* defaultMaterial);
		


	}


}

#endif //VEHICLE_H