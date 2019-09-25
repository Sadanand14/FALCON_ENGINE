#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include "EntityInterface.h"
#include "../Rendering/Mesh.h"
#include "../Core/AssetManager.h"

enum RenderPass { Terrain, Environment, ShadowMaps, Skybox, Transparent, DontDraw };

class RenderComponent: public ComponentBase 
{
	 
private:
	static const ComponentType m_type = Rendering;
	RenderPass m_renderpass;
	Model* model;

public:
	RenderComponent();
	~RenderComponent();

	inline const RenderPass GetPass() { return m_renderpass; }

	virtual void V_Update() override;
};

#endif // !1
