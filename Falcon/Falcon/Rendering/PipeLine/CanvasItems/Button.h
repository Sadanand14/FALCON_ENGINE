#ifndef BUTTON_H
#define BUTTON_H

#include "CanvasItem.h"
#include "System/Types.h"

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
		nk_color m_borderColor;
		Font* m_font;
		boost::function<void(void)> m_callback;

	public:
		Button();
		virtual ~Button();
		void Commands(nk_context* ctx) override;
		inline void SetText(std::string txt) { m_text = txt; }
		inline void SetCallback(boost::function<void(void)> callback) { m_callback = callback; }

		//Text colors
		inline void SetNormalTextColor(nk_color color) { m_textNormal = color; }
		inline void SetHoverTextColor(nk_color color) { m_textHover = color; }
		inline void SetActiveTextColor(nk_color color) { m_textActive = color; }

		//Button colors
		inline void SetNormalButtonColor(nk_color color) { m_buttonNormal = color; }
		inline void SetHoverButtonColor(nk_color color) { m_buttonHover = color; }
		inline void SetActiveButtonColor(nk_color color) { m_buttonActive = color; }

		inline void SetBorderColor(nk_color color) { m_borderColor = color; }
};

#endif