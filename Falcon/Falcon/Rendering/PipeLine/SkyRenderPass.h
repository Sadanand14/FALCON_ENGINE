#ifndef SKY_RENDER_PASS_H
#define SKY_RENDER_PASS_H
#include <Types.h>
#include "RenderPass.h"

class SkyRenderPass : public RenderPass 
{
public : 
	SkyRenderPass(u32 priority);
	void Render()override;
};

#endif // !1

