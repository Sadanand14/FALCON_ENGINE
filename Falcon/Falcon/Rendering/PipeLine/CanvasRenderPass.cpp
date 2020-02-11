#include "CanvasRenderPass.h"

#include "Canvas.h"
#include "WindowData.h"
#include "Font.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#include <nuklear.h>

CanvasRenderPass::CanvasRenderPass(uint32_t priority) : RenderPass(priority)
{
	AddEnabledProperty(GL_SAMPLE_ALPHA_TO_COVERAGE);
	AddEnabledProperty(GL_BLEND);
	AddEnabledProperty(GL_SCISSOR_TEST);
	AddDisabledProperty(GL_CULL_FACE);
	AddDisabledProperty(GL_DEPTH_TEST);

	SetCullFace(GL_BACK);
	SetBlendEquation(GL_FUNC_ADD);
	SetBlendFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetDepthMask(false);

	nk_init_default(&m_ctx, nullptr);

	m_proj = {
		{  2.0f, 0.0f, 0.0f, 0.0f },
		{  0.0f,-2.0f, 0.0f, 0.0f },
		{  0.0f, 0.0f,-1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	m_proj[0][0] /= WindowData::windowSize.x;
	m_proj[1][1] /= WindowData::windowSize.y;

	AssetManager::CreateDefaultFont();
	defaultFont = AssetManager::GetFont("default");
}

void CanvasRenderPass::Render()
{
	RenderPass::Render();

	for(auto it = m_renderQueue.begin(); it != m_renderQueue.end(); it++)
	{
		//Get the canvas
		Canvas* can = static_cast<Canvas*>(*it);

		//////////////////////
		//// Setup drawing on nk canvas

		//Save old properties
		can->SetPanelPadding(m_ctx.style.window.padding);
		can->SetItemSpacing(m_ctx.style.window.spacing);
		can->SetWindowBackground(m_ctx.style.window.fixed_background);

		//Set some vars
		nk_style_set_font(&m_ctx, defaultFont->GetFontHandle());
		m_ctx.style.window.spacing = nk_vec2(5, 5);
		m_ctx.style.window.padding = nk_vec2(20, 20);
		nk_color bgColor = nk_rgb(255, 255, 255);
		m_ctx.style.window.background = nk_rgb(255, 255, 255);
		m_ctx.style.window.fixed_background = nk_style_item_color(bgColor);

		//nk_rgb(234, 235, 238);

		//Call Draw commands
		can->CallDrawCommands(&m_ctx);
		//End drawing
		m_ctx.style.window.spacing = can->GetPanelPadding();
		m_ctx.style.window.padding = can->GetItemSpacing();
		m_ctx.style.window.fixed_background = can->GetWindowBackground();

		//////////////////////
		//// Actual GL Drawing

		//Offset for drawing
		const nk_draw_index* offset = NULL;

		//Set the shader to use
		Shader* shad = can->GetMaterial()->m_shader;
		can->Bind();
		shad->UseShader();
		glActiveTexture(GL_TEXTURE0);
		shad->SetInt("tex", 0);
		shad->SetMat4("proj", m_proj);

		//Create the vertex layout for the config to use
		static const nk_draw_vertex_layout_element vertexLayout[] = {
			{ NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(UIVertex, position) },
			{ NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(UIVertex, uv) },
			{ NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(UIVertex, color) },
			{ NK_VERTEX_LAYOUT_END}
		};

		//Create config and set attribs
		nk_convert_config config;
		memset(&config, 0, sizeof(config));
		config.vertex_layout = vertexLayout;
		config.vertex_size = sizeof(UIVertex);
		config.vertex_alignment = NK_ALIGNOF(UIVertex);
		config.null = can->GetNull();
		config.circle_segment_count = 22;
		config.curve_segment_count = 22;
		config.arc_segment_count = 22;
		config.global_alpha = 1.0f;
		config.shape_AA = NK_ANTI_ALIASING_ON;
		config.line_AA = NK_ANTI_ALIASING_ON;

		//Grab pointer to mem location on gpu
		void* vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		void* indices = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

		//Create the nk buffers
		nk_buffer verts;
		nk_buffer idx;
		nk_buffer_init_fixed(&verts, vertices, can->GetMaxVertexSize());
		nk_buffer_init_fixed(&idx, indices, can->GetMaxIndexSize());

		//Convert to GL buffers
		nk_convert(&m_ctx, can->GetCommandBuffer(), &verts, &idx, &config);

		//Unmap the buffers
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);


		const nk_draw_command* cmd;
		//Actual draw loop
		nk_draw_foreach(cmd, &m_ctx, can->GetCommandBuffer())
		{
			if (!cmd->elem_count)
				continue;
			//Set texturing and scissor test
			glBindTexture(GL_TEXTURE_2D, cmd->texture.id);
			glScissor(cmd->clip_rect.x,
					  (WindowData::windowSize.y - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)),
					  cmd->clip_rect.w,
					  cmd->clip_rect.h);

			//Draw and offset
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count;
		}
		nk_clear(&m_ctx);
	}

	//m_renderQueue.clear();
}

CanvasRenderPass::~CanvasRenderPass()
{
	nk_free(&m_ctx);
}