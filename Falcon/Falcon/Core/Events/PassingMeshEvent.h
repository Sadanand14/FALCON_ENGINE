#ifndef PASSING_RENDERING_EVENT_H
#define PASSING_RENDERING_EVENT_H

#include <PipeLine/Mesh.h>
#include <Events/Event.h>

struct PassingRenderingEvent : public Event 
{
	Mesh* m_mesh;
public: 
	PassingRenderingEvent(Mesh* mesh) : m_mesh(mesh),Event(EVENT_DATA_TO_RENDERER) {};
	inline Mesh* GetMesh()const { return m_mesh; }
};

#endif