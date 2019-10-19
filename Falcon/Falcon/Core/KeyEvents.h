#ifndef KEY_EVENTS_H
#define KEY_EVENTS_H

#include <Core/Event.h>

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
	KeyEvent(unsigned int KeyCode, keyType type);
	~KeyEvent();
};

struct KeyPressed : public KeyEvent 
{

public:
	KeyPressed(unsigned int KeyCode);
	~KeyPressed();
};

struct KeyReleased : public KeyEvent
{
public:
	KeyReleased(unsigned int KeyCode);
	~KeyReleased();
};

struct KeyRepeat : public KeyEvent
{
public:
	KeyRepeat(unsigned int KeyCode);
	~KeyRepeat();
};
#endif // !1
