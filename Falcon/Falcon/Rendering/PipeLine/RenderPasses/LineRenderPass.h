#ifndef LINERENDERPASS_H
#define LINERENDERPASS_H

#include "RenderPass.h"
#include "Line.h"

class LineRenderPass : public RenderPass
{
public:
	LineRenderPass(uint32_t priority);
	void Render() override;
};

#endif