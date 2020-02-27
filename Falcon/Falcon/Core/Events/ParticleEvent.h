#ifndef PARTICLE_EVENT_H
#define PARTICLE_EVENT_H

#include "Event.h"
#include <Log.h>

/**
*Structure Definition for Render Type Event.
*/
struct ParticleEvent : public Event
{
	ParticleEvent(): Event(EVENT_PARTICLE)
	{
		//FL_ENGINE_INFO("RenderEvent Started");
	};
};


#endif // !1

