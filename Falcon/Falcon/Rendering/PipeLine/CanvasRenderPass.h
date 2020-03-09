#ifndef CANVASRENDERPASS_H
#define CANVASRENDERPASS_H

#include "RenderPass.h"
#include "AssetManager.h"

#include "System/Types.h"

class GLFWwindow;

class CanvasRenderPass : public RenderPass
{
	private:
		nk_context m_ctx;
		glm::mat4 m_proj;
		Font* defaultFont;

	public:
		CanvasRenderPass(uint32_t priority);
		void Render() override;
		void PushInput(GLFWwindow* win);
		virtual ~CanvasRenderPass();
};

#endif