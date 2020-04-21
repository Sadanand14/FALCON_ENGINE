#ifndef CAREVENT_H
#define CAREVENT_H


#include "Event.h"
#include "KeyEvents.h"
#include "Physics/vehicle/Vehicle.h"

struct CarEvent : public Event
{
	physics::vehicle::Car* m_car;
	bool m_isUserCar;
	CarEvent(physics::vehicle::Car* car, bool isUserCar = true) :
		Event(EVENT_CAR_CREATED),
		m_car(car),
		m_isUserCar(isUserCar)
	{}
};


struct CarAIInputEvent: public Event
{
	physics::vehicle::Car* m_car;
	physics::vehicle::DriveMode m_driveMode;
	CarAIInputEvent(physics::vehicle::Car* car, physics::vehicle::DriveMode mode)
		: Event(EVENT_CAR_CREATED),m_car(car), m_driveMode(mode)
	{}

};


#endif // !CAREVENT_H