#ifndef RENDER_EVENT_H
#define RENDER_EVENT_H

#include "Event.h"
#include <System/Log.h>

/**
*Structure Definition for Render Type Event.
*/
struct RenderEvent : public Event
{
	RenderEvent(): Event(RenderEventCategory) 
	{
		//FL_ENGINE_INFO("RenderEvent Started");
	};
};


#endif // !1

