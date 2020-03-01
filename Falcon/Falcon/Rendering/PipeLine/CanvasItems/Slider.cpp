#include "Slider.h"

#include "Font.h"
#include "AssetManager.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

/**
 * Constructor for a slider
 */
Slider::Slider() : CanvasItem()
{

}

/**
 * Slider destructor
 */
Slider::~Slider()
{

}

/** The draw commands for the slider
 *
 * @param ctx - The nuklear context to draw to
 */
void Slider::Commands(nk_context* ctx)
{
	nk_slider_int(ctx, m_minValue, &m_curValue, m_maxValue, m_step);
}