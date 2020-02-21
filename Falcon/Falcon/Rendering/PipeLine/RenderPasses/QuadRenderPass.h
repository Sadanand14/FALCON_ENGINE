#ifndef  QUAD_HRENDER_HPASS_H
#define QUAD_HRENDER_HPASS_H

#include "RenderPass.h"

class QuadRenderPass : public RenderPass
{
	Shader* m_Shader;
	u32 m_texture;
public:
	QuadRenderPass(u32 priority);
	virtual void Render() override;
	virtual ~QuadRenderPass();
};

#endif
