#ifndef CAREVENT_H
#define CAREVENT_H


#include "Event.h"
#include "Physics/Physics.h"

struct CarEvent : public Event
{
	physics::vehicle::Car* m_car;
	
	CameraEvent(physics::vehicle::Car* car) :Event(EVENT_CAR_CREATED), m_car(car)
	{}
}
#endif // !1
