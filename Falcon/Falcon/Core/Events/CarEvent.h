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


struct CarInputEvent : public KeyEvent
{
	//KeyEvent(unsigned int keycode, keyType type) : m_keyCode(keycode), m_type(type), Event(EVENT_KEY_INPUT) {}
	physics::vehicle::Car* m_car;

	CarInputEvent(physics::vehicle::Car* car, unsigned int keycode, keyType type)
		: KeyEvent(EVENT_CAR_INPUT,keycode, type),
		  m_car(car)
	{}

};


#endif // !CAREVENT_H