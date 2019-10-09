#ifndef RENDER_EVENT_H
#define RENDER_EVENT_H

#include "Event.h"

struct RenderEvent : public Event
{
	RenderEvent(EventsCategory category = RenderEventCategory): Event(category) {};
};


#endif // !1

