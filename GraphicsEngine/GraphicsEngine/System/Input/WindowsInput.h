#ifndef WINDOWSINPUT_H
#define WINDOWSINPUT_H

#include "framework.h"
#include "Input.h"
#include <utility>

class WindowsInput : public Input
{

protected:
	virtual bool IsKeyDown(int keycode) override;
	virtual bool IsKeyUp(int keycode) override;
	virtual bool IsKeyRepeated(int keycode) override;
	virtual bool IsMouseButtonPressed(int button) override;
	virtual std::pair<float, float> GetMousePositionImpl() override;
	virtual float GetMouseXImpl() override;
	virtual float GetMouseYImpl() override;

};
#endif // WINDOWSINPUT_H

