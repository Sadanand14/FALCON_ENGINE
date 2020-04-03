#ifndef EVENT_H_
#define EVENT_H_
#define BIT(x) (1<<x)

#include <Types.h>
#include <iostream>
#include <functional>


/**
* Categorization using bitmasking for differentiating between event types.
*/
enum EventsCategory {
	none					 = 0,
	EVENT_WINDOWS			 = BIT(0),
	EVENT_KEY_INPUT			 = BIT(1),
	EVENT_MOUSE_INPUT		 = BIT(2),
	EVENT_RENDER			 = BIT(3),
	EVENT_PARTICLE			 = BIT(4),
	EVENT_DATA_TO_RENDERER	 = BIT(5),
	EVENT_CAMERA_COMPONENT   = BIT(6)
};


typedef std::function<void()> EventFunction;

/**
* Basic Event Structure Definition
*/
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
		//std::cout << category << std::endl;
	};
	virtual ~Event()
	{

	}
};
#endif

