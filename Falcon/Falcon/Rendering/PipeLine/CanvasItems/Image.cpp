#include "Image.h"

/**
 * Constructor for an imagae
 */
UI::Image::Image() : CanvasItem()
{

}

/**
 * Image destructor
 */
UI::Image::~Image()
{

}

/** The draw commands for the image
 *
 * @param ctx - The nuklear context to draw to
 */
void UI::Image::Commands(nk_context* ctx)
{
	nk_image(ctx, m_img);
}
