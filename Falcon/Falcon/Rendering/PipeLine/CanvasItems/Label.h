#ifndef LABEL_H
#define LABEL_H

#include <string>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#include <nuklear.h>

#include "CanvasItem.h"
#include "System/Types.h"

class Font;

/**
 * A basic label
 */
class Label : public CanvasItem
{
	protected:
		bool m_wrap;
		std::string m_text;
		nk_color m_textColor;
		nk_flags m_hAlignment;
		nk_flags m_vAlignment;
		Font* m_font;

	public:
		Label();
		virtual ~Label();
		void Commands(nk_context* ctx) override;
		inline void SetWrap(bool wrap) { m_wrap = wrap; }
		inline void SetText(std::string txt) { m_text = txt; }
		inline void SetAlignment(nk_flags alignment) { m_hAlignment = alignment; }
		inline void SetVerticalAlignment(nk_flags alignment) { m_vAlignment = alignment; }
		inline void SetTextColor(nk_color color) { m_textColor = color; }
};

#endif