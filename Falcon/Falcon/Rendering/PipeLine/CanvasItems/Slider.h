#ifndef SLIDER_H
#define SLIDER_H

#include <string>

#include "CanvasItem.h"
#include "System/Types.h"

namespace UI
{
	/**
	 * A basic Slider
	 */
	class Slider : public CanvasItem
	{
	protected:
		float m_minValue;
		float m_maxValue;
		float m_curValue;
		float m_step;
		nk_color m_sliderBarNormal;
		nk_color m_sliderBarHover;
		nk_color m_sliderBarActive;
		nk_color m_sliderBarFill;
		nk_color m_sliderCursorNormal;
		nk_color m_sliderCursorHover;
		nk_color m_sliderCursorActive;
		struct nk_vec2 m_cursorSize;
		float m_barHeight;

	public:
		Slider();
		virtual ~Slider();
		void Commands(nk_context* ctx) override;
		inline void SetMinValue(float minValue) { m_minValue = minValue; }
		inline void SetMaxValue(float maxValue) { m_maxValue = maxValue; }
		inline void SetStep(float step) { m_step = step; }
		inline float GetCurValue() { return m_curValue; }
		inline void SetBarNormalColor(glm::vec4 color) { m_sliderBarNormal = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetBarHoverColor(glm::vec4 color) { m_sliderBarHover = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetBarActiveColor(glm::vec4 color) { m_sliderBarActive = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetBarFillColor(glm::vec4 color) { m_sliderBarFill = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetCursorNormalColor(glm::vec4 color) { m_sliderCursorNormal = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetCursorHoverColor(glm::vec4 color) { m_sliderCursorHover = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetCursorActiveColor(glm::vec4 color) { m_sliderCursorActive = nk_rgba(color.r, color.g, color.b, color.a); }
		inline void SetCursorSize(glm::vec2 size) { m_cursorSize = nk_vec2(size.x, size.y); }
		inline void SetBarHeight(float height) { m_barHeight = height; }
	};
}
#endif