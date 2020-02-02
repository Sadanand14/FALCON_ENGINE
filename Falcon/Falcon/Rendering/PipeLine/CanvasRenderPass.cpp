#include "CanvasRenderPass.h"

#include "Canvas.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
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

	proj = {
		{  2.0f, 0.0f, 0.0f, 0.0f },
		{  0.0f,-2.0f, 0.0f, 0.0f },
		{  0.0f, 0.0f,-1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	proj[0][0] /= 1280;
	proj[1][1] /= 720;
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
		nk_style_set_font(&m_ctx, &can->GetFont()->handle);
		m_ctx.style.window.spacing = nk_vec2(0, 0);
		m_ctx.style.window.padding = nk_vec2(0, 0);
		nk_color bgColor = nk_rgba( 250, 250, 250, 0);
		m_ctx.style.window.fixed_background = nk_style_item_color(bgColor);

		//TODO: Make this dynamic
		int width = 1280;
		int height = 720;

		//Begin drawing
		int flags = 0 & ~NK_WINDOW_DYNAMIC;
		nk_window_set_bounds(&m_ctx, "Window", nk_rect(0, 0, width, height));
		nk_begin(&m_ctx, "Window", nk_rect(0, 0, width, height), NK_WINDOW_NO_SCROLLBAR | flags);

		struct nk_rect total_space;
		total_space = nk_window_get_content_region(&m_ctx);
		nk_layout_row_dynamic(&m_ctx, total_space.h, 1);
		nk_widget(&total_space, &m_ctx);
		can->SetPainter(nk_window_get_canvas(&m_ctx));

		//Draw the nuklear things
		nk_fill_rect(can->GetPainter(), nk_rect(15, 15, 210, 210), 5, nk_rgb(247, 230, 154));
		nk_fill_rect(can->GetPainter(), nk_rect(20, 20, 200, 200), 5, nk_rgb(188, 174, 118));
		nk_draw_text(can->GetPainter(), nk_rect(30, 30, 150, 20), "Text to draw", 12, &can->GetFont()->handle, nk_rgb( 188, 174, 118), nk_rgb(0, 0, 0));
		nk_fill_rect(can->GetPainter(), nk_rect(250, 20, 100, 100), 0, nk_rgb(0, 0, 255));
		nk_fill_circle(can->GetPainter(), nk_rect(20, 250, 100, 100), nk_rgb(255, 0, 0));
		nk_fill_triangle(can->GetPainter(), 250, 250, 350, 250, 300, 350, nk_rgb(0, 255, 0));
		//nk_fill_arc(canvas.painter, 300, 180, 50, 0, 3.141592654f * 3.0f / 4.0f, nk_rgb(255, 255, 0));


		//End drawing
		nk_end(&m_ctx);
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
		shad->SetMat4("proj", proj);

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
					  (height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)),
					  cmd->clip_rect.w,
					  cmd->clip_rect.h);

			//Draw and offset
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count;
		}
		nk_clear(&m_ctx);

		//shad->SetMat4("proj", cam->GetWorldMatrix());
	}

	//m_renderQueue.clear();
}

CanvasRenderPass::~CanvasRenderPass()
{
	nk_free(&m_ctx);
}