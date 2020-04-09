#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include <Types.h>
#include <Physics.h>
#include <vehicle/Vehicle.h>
#include <EventSystem.h>
#include <EventManager.h>

typedef physics::vehicle::Car CarStruct;
typedef boost::container::vector<CarStruct*, fmemory::STLAllocator<CarStruct*>> CarArr;


class CarEvents :public EventSystem
{
private:
	friend class CarSystem;
	virtual void SubscribeToEvents();
	
public:
	virtual void ProcessEvents();
	CarEvents();
	~CarEvents();
};

class CarSystem
{
private:
	static CarArr m_userCars;
	static CarArr m_AICars;
public:
	static inline CarArr* GetUserCars() { return &m_userCars; }
	static inline CarArr* GetAICars() { return &m_AICars; }
	static void AddCar(CarStruct* car, bool isUserCar);
	void Update();
};



#endif // !CAR_SYSTEM_H
