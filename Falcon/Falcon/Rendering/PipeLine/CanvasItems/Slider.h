#ifndef SLIDER_H
#define SLIDER_H

#include <string>

#include "CanvasItem.h"
#include "System/Types.h"

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