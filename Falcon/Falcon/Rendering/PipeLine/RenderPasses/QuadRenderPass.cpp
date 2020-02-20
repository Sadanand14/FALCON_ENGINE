#include "QuadRenderPass.h"

QuadRenderPass::QuadRenderPass(u32 priority) :RenderPass(priority)
{
	m_Shader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/Quad.vert", "../Falcon/Rendering/Shader/Quad.frag");
}

QuadRenderPass::~QuadRenderPass() 
{
	if (m_Shader != nullptr) fmemory::fdelete(m_Shader);
}