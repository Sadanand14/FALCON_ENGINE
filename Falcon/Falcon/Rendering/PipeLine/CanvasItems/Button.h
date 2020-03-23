#ifndef BUTTON_H
#define BUTTON_H

#include "CanvasItem.h"
#include "System/Types.h"
#include "AssetManager.h"

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
	inline void SetNormalTextColor(glm::vec4 color) { m_textNormal = nk_rgba(color.x, color.y, color.z, color.w); }
	inline void SetHoverTextColor(glm::vec4 color) { m_textHover = nk_rgba(color.x, color.y, color.z, color.w); }
	inline void SetActiveTextColor(glm::vec4 color) { m_textActive = nk_rgba(color.x, color.y, color.z, color.w); }

	//Button colors
	inline void SetNormalButtonColor(glm::vec4 color) { m_buttonNormal = nk_rgba(color.x, color.y, color.z, color.w); }
	inline void SetHoverButtonColor(glm::vec4 color) { m_buttonHover = nk_rgba(color.x, color.y, color.z, color.w); }
	inline void SetActiveButtonColor(glm::vec4 color) { m_buttonActive = nk_rgba(color.x, color.y, color.z, color.w); }

	inline void SetBorderColor(glm::vec4 color) { m_borderColor = nk_rgba(color.x, color.y, color.z, color.w); }
};
#endif