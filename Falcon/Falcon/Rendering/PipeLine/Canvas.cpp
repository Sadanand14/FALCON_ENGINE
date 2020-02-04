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

Canvas::Canvas() : m_VBO(nullptr), m_IBO(nullptr)
{
	//TODO: AUTO LOAD SHADERS
}

void Canvas::Setup()
{
	Renderable::Setup();

	m_material = fmemory::fnew<Material>();
	m_material->m_shader = fmemory::fnew<Shader>("Rendering/Shader/UI.vert", "Rendering/Shader/UI.frag");

	//int w;
	//int h;
 //
	////Create texture atlas
	//nk_font_atlas_init_default(&m_atlas);
	//nk_font_atlas_begin(&m_atlas);
	//m_font = nk_font_atlas_add_default(&m_atlas, 13, 0);
	//m_image = nk_font_atlas_bake(&m_atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
 //
	////Make opengl texture
	//glGenTextures(1, &m_fontTex.textureID);
	//glBindTexture(GL_TEXTURE_2D, m_fontTex.textureID);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image);
 //
	//nk_font_atlas_end(&m_atlas, nk_handle_id((int)m_fontTex.textureID), &m_null);

	//Init command buffer
	nk_buffer_init_default(&m_cmds);

	m_VBO = fmemory::fnew<VertexBuffer>(nullptr, m_maxVerts, GL_STREAM_DRAW);
	m_IBO = fmemory::fnew<IndexBuffer>(nullptr, m_maxInds, GL_STREAM_DRAW);

	m_VBO->Bind();
	m_VAO->AddVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), 0, 0);
	m_VAO->AddVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), offsetof(UIVertex, uv), 0);
	m_VAO->AddVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(UIVertex), offsetof(UIVertex, color), 0);
	m_VBO->Unbind();
}

void Canvas::Bind()
{
	Renderable::Bind();
	m_VBO->BufferData(nullptr, m_maxVerts, GL_STREAM_DRAW);
	m_IBO->BufferData(nullptr, m_maxInds, GL_STREAM_DRAW);
	m_VBO->Bind();
	m_IBO->Bind();
}

void Canvas::CallDrawCommands(nk_context* ctx)
{
	for(u32 i = 0; i < m_canvasItems.size(); i++)
		m_canvasItems[i]->Draw(ctx);
}

Canvas::~Canvas()
{
	nk_buffer_free(&m_cmds);
}