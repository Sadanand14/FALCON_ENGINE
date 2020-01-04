#include "ParticleRenderPass.h"

ParticleRenderPass::ParticleRenderPass(uint32_t priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_DEPTH_TEST);
	AddEnabledProperty(GL_CULL_FACE);
	AddEnabledProperty(GL_SAMPLE_ALPHA_TO_COVERAGE);
	AddEnabledProperty(GL_BLEND);

	SetCullFace(GL_BACK);
	SetBlendEquation(GL_FUNC_ADD);
	SetBlendFunctions(GL_SRC_ALPHA, GL_ONE);
	SetDepthMask(false);
}

void ParticleRenderPass::Render()
{
	RenderPass::Render();

	for(auto it = m_renderQueue.begin(); it != m_renderQueue.end(); it++)
	{
		Particle* particle = static_cast<Particle*>(*it);
		Shader* shad = particle->GetMaterial()->m_shader;
		shad->UseShader();
		shad->SetMat4("model", particle->GetWorldMatrix());
		particle->Bind();
		glDrawArraysInstanced(GL_POINTS, 0, 1, particle->GetParticleDataAmount());
		particle->ClearParticleData();
	}

	m_renderQueue.clear();
}