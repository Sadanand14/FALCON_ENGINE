#include "SkyRenderPass.h"
#include <PipeLine/Mesh.h>

SkyRenderPass::SkyRenderPass(u32 priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_DEPTH_TEST);
	AddEnabledProperty(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	AddEnabledProperty(GL_CULL_FACE);
	AddDisabledProperty(GL_BLEND);
	AddDisabledProperty(GL_SCISSOR_TEST);

	SetCullFace(GL_FRONT);
	SetDepthMask(GL_FALSE);
}

void SkyRenderPass::Render()
{
	RenderPass::Render();
	glDepthFunc(GL_LEQUAL);

	for (auto it = m_renderQueue.begin(); it != m_renderQueue.end(); it++)
	{
		Mesh* m = static_cast<Mesh*>(*it);
		Shader* shad = m->GetMaterial()->m_shader;
		shad->UseShader();
		m->Bind();

		for (u32 i = 0; i < m->m_indexOffsets.size(); i++)
		{
			i32 count;
			if (i < m->m_indexOffsets.size() - 1)
				count = m->m_indexOffsets[i + 1] - m->m_indexOffsets[i];
			else
				count = m->m_indexArray.size() - m->m_indexOffsets[i];
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, m->GetWorldMatrixAmount(), m->m_indexOffsets[i]);
		}
		m->ClearWorldMatrices();
	}
	m_renderQueue.clear();
}