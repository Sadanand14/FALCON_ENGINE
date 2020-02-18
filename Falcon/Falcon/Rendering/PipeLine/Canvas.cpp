#include "Canvas.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include "Texture.h"
#include "CanvasItems/CanvasItem.h"

/**
 * Canvas constructor
 */
Canvas::Canvas() : m_VBO(nullptr), m_IBO(nullptr)
{

}

/**
 * Setups up buffers and shaders for a canvas
 */
void Canvas::Setup()
{
	Renderable::Setup();

	m_material = fmemory::fnew<Material>();
	m_material->m_shader = fmemory::fnew<Shader>("Rendering/Shader/UI.vert", "Rendering/Shader/UI.frag");

	//Init command buffer
	nk_buffer_init_default(&m_cmds);

	m_VBO = fmemory::fnew<VertexBuffer>(nullptr, m_maxVerts, GL_STREAM_DRAW);
	m_IBO = fmemory::fnew<IndexBuffer>(nullptr, m_maxInds, GL_STREAM_DRAW);

	m_VBO->Bind();
	m_VAO->AddVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), 0, 0);
	m_VAO->AddVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), offsetof(UIVertex, uv), 0);
	m_VAO->AddVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(UIVertex), offsetof(UIVertex, color), 0);
	m_VBO->Unbind();
}

/**
 * Binds a canvas for rendering
 */
void Canvas::Bind()
{
	Renderable::Bind();
	m_VBO->BufferData(nullptr, m_maxVerts, GL_STREAM_DRAW);
	m_IBO->BufferData(nullptr, m_maxInds, GL_STREAM_DRAW);
	m_VBO->Bind();
	m_IBO->Bind();
}

/**
 * Calls the nuklear draw commands for all canvas items
 */
void Canvas::CallDrawCommands(nk_context* ctx)
{
	for(u32 i = 0; i < m_canvasItems.size(); i++)
		m_canvasItems[i]->Draw(ctx);
}

/**
 * Free extra memory used by the canvas
 */
Canvas::~Canvas()
{
	nk_buffer_free(&m_cmds);
	fmemory::fdelete(m_material);
}