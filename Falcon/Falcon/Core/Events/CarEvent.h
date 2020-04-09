#ifndef CAREVENT_H
#define CAREVENT_H


#include "Event.h"
#include "Physics/Physics.h"

struct CarEvent : public Event
{
	physics::vehicle::Car* m_car;
	bool m_isUserCar;
	Car(physics::vehicle::Car* car, isUserCar = true) :
		Event(EVENT_CAR_CREATED), 
		m_car(car),
		m_isUserCar(isUserCar)
	{}
}
#endif // !1
