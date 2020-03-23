#include "RenderPass.h"

RenderPass::RenderPass(uint32_t priority)
{
	m_priority = priority;
}

void RenderPass::QueueRenderable(Renderable* renderable)
{
	if (m_renderQueue.find(renderable) == m_renderQueue.end())
		m_renderQueue.insert(renderable);
}

void RenderPass::Render()
{
	for(auto propety : m_enabledProperties)
	{
		glEnable(propety);
	}

	for(auto propety : m_disabledProperties)
	{
		glDisable(propety);
	}

	glCullFace(m_cullFace);
	glDepthMask(m_depthMask);
	glBlendEquation(m_blendEquation);
	glBlendFunc(m_srcBlendFunc, m_dstBlendFunc);
}

RenderPass::~RenderPass()
{

}