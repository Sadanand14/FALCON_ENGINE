#ifndef CANVASRENDERPASS_H
#define CANVASRENDERPASS_H

#include "RenderPass.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

class CanvasRenderPass : public RenderPass
{
	private:
		nk_context m_ctx;
		glm::mat4 proj;

	public:
		CanvasRenderPass(uint32_t priority);
		void Render() override;
		~CanvasRenderPass();
};

#endif