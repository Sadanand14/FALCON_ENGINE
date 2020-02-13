#ifndef BUTTON_H
#define BUTTON_H

#include "CanvasItem.h"

class Font;

class Button : public CanvasItem
{
	protected:
		bool m_wrap;
		std::string m_text;
		//nk_flags m_hAlignment;
		//nk_flags m_vAlignment;
		Font* m_font;
		void (*m_callback)(void);

	public:
		Button(const char* name);
		virtual ~Button();
		void Commands(nk_context* ctx) override;
		//inline void SetWrap(bool wrap) { m_wrap = wrap; }
		inline void SetText(std::string txt) { m_text = txt; }
		//inline void SetAlignment(nk_flags alignment) { m_hAlignment = alignment; }
		//inline void SetVerticalAlignment(nk_flags alignment) { m_vAlignment = alignment; }
		inline void SetCallback(void (*callback)(void)) { m_callback = callback; }
};

#endif