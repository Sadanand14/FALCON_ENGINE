#include "LineRenderPass.h"

LineRenderPass::LineRenderPass(uint32_t priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_CULL_FACE);
	AddDisabledProperty(GL_SCISSOR_TEST);
	AddDisabledProperty(GL_DEPTH_TEST);
	AddDisabledProperty(GL_BLEND);
	AddDisabledProperty(GL_SAMPLE_ALPHA_TO_COVERAGE);

	SetCullFace(GL_BACK);
	//SetBlendEquation(GL_FUNC_ADD);
	//SetBlendFunctions(GL_SRC_ALPHA, GL_ONE);
	//SetDepthMask(false);
}

void LineRenderPass::Render()
{
	RenderPass::Render();

	for(auto it = m_renderQueue.begin(); it != m_renderQueue.end(); it++)
	{
		Line* line = static_cast<Line*>(*it);
		Shader* shad = line->GetMaterial()->m_shader;
		shad->UseShader();
		shad->SetVec3("color", line->GetColor());

		line->Bind();
		switch(line->GetLineType())
		{
			case LineType::LOOP:
				glDrawArrays(GL_LINE_LOOP, 0, line->GetPointAmount());
				break;
			case LineType::STRIP:
				glDrawArrays(GL_LINE_STRIP, 0, line->GetPointAmount());
				break;
			case LineType::LINE:
			default:
				glDrawArrays(GL_LINES, 0, line->GetPointAmount());
				break;
		}
		//line->ClearPoints();
	}

	m_renderQueue.clear();
}