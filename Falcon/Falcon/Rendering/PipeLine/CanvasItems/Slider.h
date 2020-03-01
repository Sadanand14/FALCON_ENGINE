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
		i32 m_minValue;
		i32 m_maxValue;
		i32 m_curValue;
		i32 m_step;

	public:
		Slider();
		virtual ~Slider();
		void Commands(nk_context* ctx) override;
		inline void SetMinValue(i32 minValue) { m_minValue = minValue; }
		inline void SetMaxValue(i32 maxValue) { m_maxValue = maxValue; }
		inline void SetStep(i32 step) { m_step = step; }
		inline i32 GetCurValue() { return m_curValue; }
};

#endif