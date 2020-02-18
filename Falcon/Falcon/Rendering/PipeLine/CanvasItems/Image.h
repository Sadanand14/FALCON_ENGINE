#ifndef IMAGE_H
#define IMAGE_H

#include <string>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include "CanvasItem.h"
#include "System/Types.h"
#include "Texture.h"

/**
 * A basic label
 */
class Image : public CanvasItem
{
	protected:
		struct nk_image m_img;
		//bool m_wrap;
		//std::string m_text;
		//nk_color m_textColor;
		//nk_flags m_hAlignment;
		//nk_flags m_vAlignment;
		//Font* m_font;

	public:
		Image(const char* name);
		virtual ~Image();
		void Commands(nk_context* ctx) override;
		inline void SetImage(Texture tex) { m_img = nk_image_id(tex.textureID); }
};

#endif