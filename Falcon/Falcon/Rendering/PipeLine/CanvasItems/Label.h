#ifndef LABEL_H
#define LABEL_H

#include <string>

#include "CanvasItem.h"
#include "System/Types.h"
#include "AssetManager.h"

using namespace std;
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
	inline void SetTextColor(glm::vec4 color) { m_textColor = nk_rgba(color.x, color.y, color.z,color.w); }
};
#endif