#include "Font.h"
#define _CRT_SECURE_NO_WARNINGS
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

Font::Font(nk_font_atlas atlas, nk_font* font)
{
	int w;
	int h;

	m_atlas = atlas;
	m_font = font;
	m_image = nk_font_atlas_bake(&m_atlas, &w, &h, NK_FONT_ATLAS_RGBA32);

	//Make opengl texture
	glGenTextures(1, &m_fontTex.textureID);
	glBindTexture(GL_TEXTURE_2D, m_fontTex.textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_image);

	nk_font_atlas_end(&m_atlas, nk_handle_id((int)m_fontTex.textureID), &m_null);
}

Font::~Font()
{
	glDeleteTextures(1, &m_fontTex.textureID);
	nk_font_atlas_clear(&m_atlas);
}