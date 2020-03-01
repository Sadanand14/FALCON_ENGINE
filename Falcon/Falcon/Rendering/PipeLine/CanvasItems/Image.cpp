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
 */
Image::Image() : CanvasItem()
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
	//nk_layout_row_push(ctx, m_bounds.h);
	//nk_widget(&m_bounds, ctx);
	//nk_layout_row_dynamic(ctx, m_bounds.h, 1);
	//nk_layout_space_push(ctx, m_bounds);
	nk_image(ctx, m_img);
}