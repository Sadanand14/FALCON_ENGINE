#include "KeyEvents.h"

/////////KeyEvent

KeyEvent::KeyEvent(unsigned int keycode, keyType type): m_keyCode(keycode), m_type(type), Event(EVENT_KEY_INPUT)
{}

KeyEvent::~KeyEvent() 
{}
////////////KeyPressed Event
KeyPressed::KeyPressed(unsigned int keycode) : KeyEvent(keycode, keyType::Pressed)
{}

KeyPressed::~KeyPressed() 
{}

///////////KeyReleased Event
KeyReleased::KeyReleased(unsigned int keycode) : KeyEvent(keycode, keyType::Released)
{}

KeyReleased::~KeyReleased()
{}

///////////KeyRepeat Event
KeyRepeat::KeyRepeat(unsigned int keycode) : KeyEvent(keycode, keyType::Repeat)
{}

KeyRepeat::~KeyRepeat() 	
{}
