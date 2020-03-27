#include "Canvas.h"

#include "Texture.h"
#include "CanvasItems/CanvasItem.h"
#include "WindowData.h"

/**
 * Canvas constructor
 */
UI::Canvas::Canvas() : m_VBO(nullptr), m_IBO(nullptr)
{

}

/**
 * Setups up buffers and shaders for a canvas
 */
void UI::Canvas::Setup()
{
	Renderable::Setup();

	m_material = fmemory::fnew<Material>();
	m_material->m_shader = fmemory::fnew<Shader>("../Falcon/Rendering/Shader/UI.vert", "../Falcon/Rendering/Shader/UI.frag");

	//m_material->m_shader->UseShader();
	uint8_t data[3] = { 255, 255, 255 };

	m_material->m_albedoTex.type = TextureType::TEXTURE2D;
	glGenTextures(1, &m_material->m_albedoTex.textureID);
	glBindTexture(GL_TEXTURE_2D, m_material->m_albedoTex.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &data);
	glBindTexture(GL_TEXTURE_2D, 0);

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
void UI::Canvas::Bind()
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
void UI::Canvas::CallDrawCommands(nk_context* ctx)
{
	nk_window_set_bounds(ctx, "Canvas", nk_rect(0, 0, WindowData::windowSize.x, WindowData::windowSize.y));
	nk_begin(ctx, "Canvas", nk_rect(0, 0, WindowData::windowSize.x, WindowData::windowSize.y), NK_WINDOW_NO_SCROLLBAR);
	nk_layout_space_begin(ctx, NK_DYNAMIC, WindowData::windowSize.y, INT_MAX);

	for (u32 i = 0; i < m_canvasItems.size(); i++)
		m_canvasItems[i]->Draw(ctx);

	nk_layout_space_end(ctx);
	nk_end(ctx);
}

/**
 * Free extra memory used by the canvas
 */
UI::Canvas::~Canvas()
{
	nk_buffer_free(&m_cmds);
	fmemory::fdelete(m_material);
}