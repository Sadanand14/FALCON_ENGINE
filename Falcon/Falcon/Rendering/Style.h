#ifndef STYLE_H
#define STYLE_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

struct Style
{
	struct nk_vec2 m_itemSpacing;
	struct nk_vec2 m_panelPadding;
	nk_style_item m_windowBackground;
};


#endif