#include "TransparentRenderPass.h"

TransparentRenderPass::TransparentRenderPass(uint32_t priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_DEPTH_TEST);
	AddEnabledProperty(GL_BLEND);
	AddEnabledProperty(GL_CULL_FACE);
	AddDisabledProperty(GL_SCISSOR_TEST);

	SetCullFace(GL_BACK);
	SetBlendEquation(GL_FUNC_ADD);
	SetBlendFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetDepthMask(true);
}

void TransparentRenderPass::Render()
{
	RenderPass::Render();

	int counter = 0;
	for(u32 i = 0; i < renderables.size(); i++)
	{
		Mesh* m = static_cast<Mesh*>(renderables[i]);
		Shader* shad = m->GetMaterial()->m_shader;
		shad->UseShader();
		m->Bind();

		for(u32 j = 0; j < m->m_indexOffsets.size(); j++)
		{
			i32 count;
			if (j < m->m_indexOffsets.size() - 1)
				count = m->m_indexOffsets[j + 1] - m->m_indexOffsets[j];
			else
				count = m->m_indexArray.size() - m->m_indexOffsets[j];
			glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, m_counts[counter], m->m_indexOffsets[j], m_offsets[counter]);
		}
		counter++;
	}

	for(u32 i = 0; i < renderables.size(); i++)
		static_cast<Mesh*>(renderables[i])->ClearWorldMatrices();

	renderables.clear();
	m_counts.clear();
	m_offsets.clear();
}

void TransparentRenderPass::AddCountAndOffset(u32 count, u32 offset)
{
	m_counts.push_back(count);
	m_offsets.push_back(offset);
}

void TransparentRenderPass::QueueRenderable(Renderable* renderable)
{
	renderables.push_back(renderable);
}

TransparentRenderPass::~TransparentRenderPass()
{

}