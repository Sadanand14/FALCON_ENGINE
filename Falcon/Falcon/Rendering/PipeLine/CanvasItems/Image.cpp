#include "Image.h"


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
	nk_image(ctx, m_img);
}
