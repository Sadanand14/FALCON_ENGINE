#include "Label.h"

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
Label::Label(const char* name) : CanvasItem(name), m_wrap(false), m_hAlignment(NK_TEXT_ALIGN_LEFT),
								 m_vAlignment(NK_TEXT_ALIGN_MIDDLE), m_font(AssetManager::GetFont("default"))
{

}

/**
 * Label destructor
 */
Label::~Label()
{

}

/** The draw commands for the label
 *
 * @param ctx - The nuklear context to draw to
 */
void Label::Commands(nk_context* ctx)
{
	nk_layout_row_dynamic(ctx, m_bounds.h, 1);
	nk_widget(&m_bounds, ctx);
	if(m_wrap)
		nk_label_colored_wrap(ctx, m_text.c_str(), m_textColor);
	else
		nk_label_colored(ctx, m_text.c_str(), m_hAlignment | m_vAlignment, m_textColor);
}