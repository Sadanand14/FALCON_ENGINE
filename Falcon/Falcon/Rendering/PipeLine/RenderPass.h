#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "System/Types.h"
#include "Shader.h"
#include "Renderable.h"

class RenderPass
{
private:
	bool m_depthMask = false;
	uint32_t m_cullFace = GL_BACK;
	uint32_t m_blendEquation = GL_FUNC_ADD;
	uint32_t m_srcBlendFunc = GL_SRC_ALPHA;
	uint32_t m_dstBlendFunc = GL_ONE_MINUS_SRC_ALPHA;
	uint32_t m_priority = 0;

	boost::container::vector<uint32_t> m_enabledProperties;
	boost::container::vector<uint32_t> m_disabledProperties;

protected:
	boost::container::set<Renderable*> m_renderQueue;

public:
	RenderPass(uint32_t priority);
	virtual void QueueRenderable(Renderable* rend);
	virtual void Render();
	inline void SetDepthMask(bool isEnabled) { m_depthMask = isEnabled; }
	inline void AddEnabledProperty(uint32_t property) { m_enabledProperties.push_back(property); }
	inline void AddDisabledProperty(uint32_t property) { m_disabledProperties.push_back(property); }
	inline void SetCullFace(uint32_t cullFace) { m_cullFace = cullFace; }
	inline void SetBlendEquation(uint32_t blendEq) { m_blendEquation = blendEq; }
	inline void SetBlendFunctions(uint32_t srcFunc, uint32_t dstFunc) { m_srcBlendFunc = srcFunc; m_dstBlendFunc = dstFunc; }
	virtual ~RenderPass();
};

#endif