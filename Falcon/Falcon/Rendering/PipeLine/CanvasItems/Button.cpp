#include "Button.h"

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
 * Constructor for a label
 *
 * @param name - The name for the nuklear window
 */
Button::Button() : CanvasItem(), m_wrap(false),  m_font(AssetManager::GetFont("default")),
								   m_buttonNormal(nk_rgb(188, 188, 188)), m_buttonHover(nk_rgb(255, 255, 255)),
								   m_buttonActive(nk_rgb(130, 130, 130)), m_textNormal(nk_rgb(0, 0, 0)),
								   m_textHover(nk_rgb(0, 0, 0)), m_textActive(nk_rgb(0, 0, 0))
{

}

/**
 * Button destructor
 */
Button::~Button()
{

}

/** The draw commands for the label
 *
 * @param ctx - The nuklear context to draw to
 */
void Button::Commands(nk_context* ctx)
{
	ctx->style.button.normal = nk_style_item_color(m_buttonNormal);
	ctx->style.button.hover = nk_style_item_color(m_buttonHover);
	ctx->style.button.active = nk_style_item_color(nk_rgb_f(0.1, 0.1, 0.1));
	//ctx->style.button.border_color = nk_rgb(60,60,60);
	ctx->style.button.text_background = m_color;
	ctx->style.button.text_normal = m_textNormal;
	ctx->style.button.text_hover = m_textHover;
	ctx->style.button.text_active = m_textActive;

	if(nk_button_label(ctx, m_text.c_str()))
		m_callback();
}