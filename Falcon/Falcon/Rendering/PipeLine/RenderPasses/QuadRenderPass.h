#ifndef  QUAD_HRENDER_HPASS_H
#define QUAD_HRENDER_HPASS_H

#include "RenderPass.h"

class QuadRenderPass : public RenderPass
{
	Shader* m_Shader;
public:
	QuadRenderPass(u32 priority);
	virtual ~QuadRenderPass();
};

#endif
