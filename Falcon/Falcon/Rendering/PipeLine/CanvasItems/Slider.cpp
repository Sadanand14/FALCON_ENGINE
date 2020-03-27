#include "Slider.h"

#include "Font.h"
#include "AssetManager.h"

/**
 * Constructor for a slider
 */
UI::Slider::Slider() : CanvasItem(), m_sliderBarNormal(nk_rgb(156, 156, 156)),
					   m_sliderBarHover(nk_rgb(156, 156, 156)), m_sliderBarActive(nk_rgb(156, 156, 156)),
					   m_sliderBarFill(nk_rgb(200, 200, 200)), m_sliderCursorNormal(nk_rgb(156, 156, 156)),
					   m_sliderCursorHover(nk_rgb(200, 200, 200)), m_sliderCursorActive(nk_rgb(200, 200, 200)),
					   m_cursorSize(nk_vec2(16.0f, 16.0f)), m_barHeight(8.0f)
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
	ctx->style.slider.normal = nk_style_item_hide();
	ctx->style.slider.hover = nk_style_item_hide();
	ctx->style.slider.active = nk_style_item_hide();
	ctx->style.slider.bar_normal = m_sliderBarNormal;
	ctx->style.slider.bar_hover = m_sliderBarHover;
	ctx->style.slider.bar_active = m_sliderBarActive;
	ctx->style.slider.bar_filled = m_sliderBarFill;
	ctx->style.slider.cursor_normal = nk_style_item_color(m_sliderCursorNormal);
	ctx->style.slider.cursor_hover = nk_style_item_color(m_sliderCursorHover);
	ctx->style.slider.cursor_active = nk_style_item_color(m_sliderBarActive);
	ctx->style.slider.cursor_size = m_cursorSize;
	ctx->style.slider.bar_height = m_barHeight;

	//ctx->style.progress.normal = nk_style_item_color(m_sliderBarNormal);
	//ctx->style.progress.hover = nk_style_item_color(m_sliderBarHover);
	//ctx->style.progress.active = nk_style_item_color(m_sliderBarActive);

	nk_slider_float(ctx, m_minValue, &m_curValue, m_maxValue, m_step);
}