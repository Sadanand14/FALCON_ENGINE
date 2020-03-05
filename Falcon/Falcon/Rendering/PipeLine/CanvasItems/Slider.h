#ifndef SLIDER_H
#define SLIDER_H

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

/**
 * A basic Slider
 */
class Slider: public CanvasItem
{
	protected:
		float m_minValue;
		float m_maxValue;
		float m_curValue;
		float m_step;

	public:
		Slider();
		virtual ~Slider();
		void Commands(nk_context* ctx) override;
		inline void SetMinValue(float minValue) { m_minValue = minValue; }
		inline void SetMaxValue(float maxValue) { m_maxValue = maxValue; }
		inline void SetStep(float step) { m_step = step; }
		inline float GetCurValue() { return m_curValue; }
};

#endif