#include "Button.h"

#include "Font.h"

/**
 * Constructor for a label
 */
UI::Button::Button() : CanvasItem(), m_wrap(false), m_font(AssetManager::GetFont("default")),
m_buttonNormal(nk_rgb(188, 188, 188)), m_buttonHover(nk_rgb(255, 255, 255)),
m_buttonActive(nk_rgb(130, 130, 130)), m_textNormal(nk_rgb(0, 0, 0)),
m_textHover(nk_rgb(0, 0, 0)), m_textActive(nk_rgb(0, 0, 0)), m_borderColor(nk_rgba(0, 0, 0, 0))
{

}

/**
 * Button destructor
 */
UI::Button::~Button()
{

}

/** The draw commands for the label
 *
 * @param ctx - The nuklear context to draw to
 */
void UI::Button::Commands(nk_context* ctx)
{
	ctx->style.button.normal = nk_style_item_color(m_buttonNormal);
	ctx->style.button.hover = nk_style_item_color(m_buttonHover);
	ctx->style.button.active = nk_style_item_color(nk_rgb_f(0.1, 0.1, 0.1));
	ctx->style.button.text_background = m_color;
	ctx->style.button.text_normal = m_textNormal;
	ctx->style.button.text_hover = m_textHover;
	ctx->style.button.text_active = m_textActive;
	ctx->style.button.border_color = m_borderColor;

	if (nk_button_label(ctx, m_text.c_str()))
		m_callback();
}
