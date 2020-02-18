#include "Image.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

/**
 * Constructor for an imagae
 *
 * @param name - The name for the nuklear window
 */
Image::Image(const char* name) : CanvasItem(name)
{

}

/**
 * Image destructor
 */
Image::~Image()
{

}

/** The draw commands for the image
 *
 * @param ctx - The nuklear context to draw to
 */
void Image::Commands(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, m_bounds.h, 1);
	nk_image(ctx, m_img);
	//if(m_wrap)
	//	nk_label_colored_wrap(ctx, m_text.c_str(), m_textColor);
	//else
	//	nk_label_colored(ctx, m_text.c_str(), m_hAlignment | m_vAlignment, m_textColor);
}