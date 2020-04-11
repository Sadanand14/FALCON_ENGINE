#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include "Event.h"

/**
* Catergorization for Key Event Types
*/
enum class keyType {Released, Pressed, Repeat};

/**
*Structure Definition for Key type Events.
*/
struct KeyEvent :public Event
{
private:
	unsigned int m_keyCode;
	keyType m_type;

public: 

	inline unsigned int GetCode() { return m_keyCode; }
	inline keyType GetKeyType() { return m_type; }
	KeyEvent(unsigned int keycode, keyType type) : m_keyCode(keycode), m_type(type), Event(EVENT_KEY_INPUT){}
	~KeyEvent() {}
};

#endif// !1
