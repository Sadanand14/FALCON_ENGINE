#include "MeshRenderPass.h"

MeshRenderPass::MeshRenderPass(uint32_t priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_DEPTH_TEST);
	AddEnabledProperty(GL_CULL_FACE);
	AddDisabledProperty(GL_BLEND);

	SetCullFace(GL_BACK);
	SetDepthMask(true);
}

void MeshRenderPass::Render()
{
	RenderPass::Render();

	for(auto it = m_renderQueue.begin(); it != m_renderQueue.end(); it++)
	{
		Mesh* m = static_cast<Mesh*>(*it);
		Shader* shad = m->GetMaterial()->m_shader;
		shad->UseShader();
		m->Bind();

		for(u32 i = 0; i < m->m_indexOffsetCount; i++)
		{
			i32 count;
			if (i < m->m_indexOffsetCount - 1)
				count = m->m_indexOffsets[i + 1] - m->m_indexOffsets[i];
			else
				count = m->m_indexCount - m->m_indexOffsets[i];
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, m->GetWorldMatrixAmount(), m->m_indexOffsets[i]);
		}
		m->ClearWorldMatrices();
	}
	m_renderQueue.clear();
}