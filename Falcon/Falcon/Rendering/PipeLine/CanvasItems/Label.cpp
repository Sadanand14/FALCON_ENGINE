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

Label::Label() : m_wrap(false), m_hAlignment(NK_TEXT_ALIGN_LEFT), m_vAlignment(NK_TEXT_ALIGN_MIDDLE),
				 m_font(AssetManager::GetFont("default"))
{

}

Label::~Label()
{

}

void Label::Draw(nk_context* ctx)
{
	nk_layout_row_begin(ctx, NK_DYNAMIC, m_size.y, 1);
	nk_layout_row_push(ctx, 60);
	//nk_layout_row_dynamic(ctx, m_size.x, 1);
	nk_style_set_font(ctx, m_font->GetFontHandle());

	if(m_wrap)
		nk_label_colored_wrap(ctx, m_text.c_str(), m_color);
	else
		nk_label_colored(ctx, m_text.c_str(), m_hAlignment | m_vAlignment, m_color);
	nk_layout_row_end(ctx);
}

