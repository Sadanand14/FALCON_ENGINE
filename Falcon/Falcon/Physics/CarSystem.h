#ifndef CAR_SYSTEM_H
#define CAR_SYSTEM_H

#include <Types.h>
#include <Physics.h>
#include <vehicle/Vehicle.h>
#include <EventSystem.h>
#include <EventManager.h>

typedef physics::vehicle::Car CarStruct;
typedef boost::container::vector<CarStruct*> CarArr;
typedef physics::vehicle::DriveMode DriveMode;

class CarEventSystem :public EventSystem
{
private:
	friend class CarSystem;
	virtual void SubscribeToEvents();
	
public:
	virtual void ProcessEvents();
	CarEventSystem();
	~CarEventSystem();
};

class CarSystem
{
private:
	static CarEventSystem* m_carEvents;
	static CarArr m_userCars;
	static CarArr m_AICars;
	static CarStruct* m_currentCar;
public:
	static inline CarArr* GetUserCars() { return &m_userCars; }
	static inline CarArr* GetAICars() { return &m_AICars; }

	static void Initialize();
	static void ShutDown();
	static void AddCar(CarStruct* car, bool isUserCar);
	static void Update();
	static void ApplyInputToUserCar(DriveMode driveMode);
};



#endif // !CAR_SYSTEM_H
