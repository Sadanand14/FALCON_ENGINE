#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "BasicComponent.h"

#include <PipeLine/Mesh.h>

/**
*Structure Definition for holding data needed to render the entity.
*/
struct RenderComponent: public BasicComponent
{
	Mesh* m_mesh;

	RenderComponent() : m_mesh(nullptr) {}
	~RenderComponent() {}
};

#endif // !1
