#include "Label.h"

#include "Font.h"
#include "AssetManager.h"

/**
 * Constructor for a label
 */
Label::Label() : CanvasItem(), m_wrap(false), m_hAlignment(NK_TEXT_ALIGN_LEFT),
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
	if (m_wrap)
		nk_label_colored_wrap(ctx, m_text.c_str(), m_textColor);
	else
		nk_label_colored(ctx, m_text.c_str(), m_hAlignment | m_vAlignment, m_textColor);
}
