#ifndef BUTTON_H
#define BUTTON_H

#include <boost/function.hpp>
#include "CanvasItem.h"

class Font;

class Button : public CanvasItem
{
	protected:
		bool m_wrap;
		std::string m_text;
		nk_color m_buttonNormal;
		nk_color m_buttonHover;
		nk_color m_buttonActive;
		nk_color m_textNormal;
		nk_color m_textHover;
		nk_color m_textActive;
		//nk_flags m_hAlignment;
		//nk_flags m_vAlignment;
		Font* m_font;
		boost::function<void(void)> m_callback;

	public:
		Button();
		virtual ~Button();
		void Commands(nk_context* ctx) override;
		//inline void SetWrap(bool wrap) { m_wrap = wrap; }
		inline void SetText(std::string txt) { m_text = txt; }
		//inline void SetAlignment(nk_flags alignment) { m_hAlignment = alignment; }
		//inline void SetVerticalAlignment(nk_flags alignment) { m_vAlignment = alignment; }
		inline void SetCallback(boost::function<void(void)> callback) { m_callback = callback; }

		//Text colors
		inline void SetNormalTextColor(nk_color color) { m_textNormal = color; }
		inline void SetHoverTextColor(nk_color color) { m_textHover = color; }
		inline void SetActiveTextColor(nk_color color) { m_textActive = color; }

		//Button colors
		inline void SetNormalButtonColor(nk_color color) { m_buttonNormal = color; }
		inline void SetHoverButtonColor(nk_color color) { m_buttonHover = color; }
		inline void SetActiveButtonColor(nk_color color) { m_buttonActive = color; }


};

#endif