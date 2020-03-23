#include "QuadRenderPass.h"
#include "AssetManager.h"
QuadRenderPass::QuadRenderPass(u32 priority) :RenderPass(priority)
{
	m_texture = AssetManager::LoadTexture("../Assets/Textures/water.jpg");
	m_Shader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/Quad.vert", "../Falcon/Rendering/Shader/Quad.frag");
}

QuadRenderPass::~QuadRenderPass() 
{
	if (m_Shader != nullptr) fmemory::fdelete(m_Shader);
}

void QuadRenderPass::Render() 
{
	RenderPass::Render();
	m_Shader->UseShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}