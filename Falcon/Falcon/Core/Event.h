#ifndef EVENT_H_
#define EVENT_H_
#define BIT(x) (1<<x)

#include "../System/Types.h"
#include <iostream>
#include <functional>


// Rip-off category handling from cherno's Event System
enum EventsCategory {
	none					 = 0,
	WindowsEventCategory	 = BIT(0),
	KeyEventCategory		 = BIT(1),
	MouseEventCategory		 = BIT(2),
	RenderEventCategory		 = BIT(3)
};


typedef std::function<void()> EventFunction;

struct Event
{
	u32 m_eventFlag;

public:

	inline void SetFlag(EventsCategory category) 
	{
		m_eventFlag = (m_eventFlag | category);
	}

	inline u32 GetFlag()
	{
		return m_eventFlag;
	}

	inline bool CheckCategory(EventsCategory referenceType)
	{
		return (m_eventFlag & referenceType);
	}

	Event(EventsCategory category):m_eventFlag(category) 
	{
		std::cout << category << std::endl;
	};
	virtual ~Event()
	{

	}
};
#endif

