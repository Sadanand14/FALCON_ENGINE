#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include <Types.h>
#include <Physics.h>
#include <vehicle/Vehicle.h>

typedef physics::vehicle::Car CarStruct;
typedef boost::container::vector<CarStruct*, fmemory::STLAllocator<CarStruct*>> CarArr;

class CarSystem
{
private:
	CarArr m_userCars;
	CarArr m_AICars;

public:
	inline CarArr* GetUserCars() { return &m_userCars; }
	inline CarArr* GetAICars() { return &m_AICars; }
	void Update();
};



#endif // !CAR_SYSTEM_H
