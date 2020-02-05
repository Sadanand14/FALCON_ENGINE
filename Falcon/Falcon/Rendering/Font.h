#ifndef FONT_H
#define FONT_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include <glad.h>
#include <PipeLine/Texture.h>

class Font
{
	private:
		Texture m_fontTex;
		nk_font_atlas m_atlas;
		nk_font* m_font;
		const void* m_image;
		nk_draw_null_texture m_null;
	public:
		Font(nk_font_atlas atlas, nk_font* font);
		~Font();

		inline nk_user_font* GetFontHandle() { return &m_font->handle; }
};

#endif