#ifndef VEHICLE_INPUT_HANDLER_H
#define VEHICLE_INPUT_HANDLER_H

#include "Physx/physx/include/vehicle/PxVehicleUtilControl.h"
namespace physics
{
	class FLVehicleDrive4WRawInputData : public physx::PxVehicleDrive4WRawInputData
	{
	public:
		~FLVehicleDrive4WRawInputData() override{};
	};
}



#endif