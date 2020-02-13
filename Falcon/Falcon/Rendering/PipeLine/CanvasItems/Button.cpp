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
Button::Button(const char* name) : CanvasItem(name), m_wrap(false),  m_font(AssetManager::GetFont("default"))
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
	nk_layout_row_dynamic(ctx, m_bounds.h, 1);
	//if(m_wrap)
	//	nk_label_colored_wrap(ctx, m_text.c_str(), m_color);
	//else
	//	nk_label_colored(ctx, m_text.c_str(), m_hAlignment | m_vAlignment, m_color);
	if(nk_button_label(ctx, m_text.c_str()))
		m_callback();
}