#ifndef EVENT_H_
#define EVENT_H_
#define BIT(x) (1<<x)

#include "types.h"

#include <functional>


// Rip-off category handling from cherno's Event System
enum EventsCategory {
	none			 = 0,
	WindowsEvent	 = BIT(0),
	KeyEvent		 = BIT(1),
	MouseEvent		 = BIT(2),
	RenderEvent		 = BIT(3)
};


typedef std::function<void()> EventFunction;

class Event
{
	EventFunction m_task;
	size_t m_eventFlag;

public:

	inline void SetTask(EventFunction task)
	{
		m_task = task;
	}

	inline void SetFlag(EventsCategory category) 
	{
		m_eventFlag = m_eventFlag | category;
	}

	inline size_t GetFlag() 
	{
		return m_eventFlag;
	}

	inline EventFunction GetTask() 
	{	
		return m_task;
	}

	inline bool CheckCategory(EventsCategory referenceType)
	{
		return m_eventFlag & referenceType;
	}

	Event():m_eventFlag(0) {};
	virtual ~Event()
	{

	}
};
#endif

