#ifndef RENDER_EVENT_H
#define RENDER_EVENT_H

#include "Event.h"
#include <Log.h>

/**
*Structure Definition for Render Type Event.
*/
struct RenderEvent : public Event
{
	RenderEvent(): Event(EVENT_RENDER) 
	{
		//FL_ENGINE_INFO("RenderEvent Started");
	};
};


#endif // !1

