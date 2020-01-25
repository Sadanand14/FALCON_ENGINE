#ifndef VEHICLE_H
#define VEHICLE_H

#include "physics.h"
#include "Physx/physx/include/vehicle/PxVehicleSDK.h"
namespace physics
{

	namespace vehicle
	{
		bool InitVehicleSDK();
		bool ReleaseVehcileSDK();


		//May be a class to give an API for vehicle creation
	}


}

#endif //VEHICLE_H