#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H

#include <Events/Event.h>
#include <Types.h>

enum class Mouse_Type {ClickEvent, CursorEvent, ScrollEvent};
enum class ClickType {pressed, released, repeat};

struct MouseEvent :public Event 
{
private:
	Mouse_Type m_type;
public:
	inline Mouse_Type GetType() { return m_type; }
	MouseEvent(Mouse_Type type):Event(EVENT_MOUSE_INPUT), m_type(type) {}
};

struct MouseCursorEvent : public MouseEvent 
{
private:
	glm::vec2 m_offset;
public:
	MouseCursorEvent(glm::vec2 offset) : MouseEvent(Mouse_Type::CursorEvent), m_offset(offset)
	{}
	inline glm::vec2 GetOffset() { return m_offset;}
	
};

struct MouseClickEvent : public MouseEvent
{
private:
	int m_ButtonCode;
	Mouse_Type m_type;
	ClickType m_clktype;
public:
	inline ClickType GetClick() { return m_clktype; }
	inline Mouse_Type GetType() { return m_type; }
	inline int GetCode() { return m_ButtonCode; }
	MouseClickEvent(int code, ClickType clktype) :MouseEvent(Mouse_Type::CursorEvent), 
		m_clktype(clktype),m_ButtonCode(code)
	{}
};

struct MouseScrollEvent : public MouseEvent
{
	Mouse_Type m_type;
	inline Mouse_Type GetType() { return m_type; }
};

#endif//