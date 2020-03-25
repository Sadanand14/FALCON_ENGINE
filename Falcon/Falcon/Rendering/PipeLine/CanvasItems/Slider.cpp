#include "Slider.h"

#include "Font.h"
#include "AssetManager.h"

/**
 * Constructor for a slider
 */
UI::Slider::Slider() : CanvasItem()
{

}

/**
 * Slider destructor
 */
UI::Slider::~Slider()
{

}

/** The draw commands for the slider
 *
 * @param ctx - The nuklear context to draw to
 */
void UI::Slider::Commands(nk_context* ctx)
{
	nk_slider_float(ctx, m_minValue, &m_curValue, m_maxValue, m_step);
}