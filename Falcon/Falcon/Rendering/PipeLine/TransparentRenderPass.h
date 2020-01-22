#ifndef TRANSPARENTRENDERPASS_H
#define TRANSPARENTRENDERPASS_H

#include "RenderPass.h"
#include "Mesh.h"

class TransparentRenderPass : public RenderPass
{
private:
	boost::container::vector<u32> m_counts;
	boost::container::vector<u32> m_offsets;
	boost::container::vector<Renderable*> renderables;

public:
	virtual ~TransparentRenderPass();

	TransparentRenderPass(uint32_t priority);
	void Render() override;
	void AddCountAndOffset(u32 count, u32 offset);
	virtual void QueueRenderable(Renderable* renderable) override;
};

#endif