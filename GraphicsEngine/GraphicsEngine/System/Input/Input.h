#ifndef INPUT_H
#define INPUT_H

#include "framework.h"

/* TODO:
/  1. Add GetAxis like method.
/  2. More methods around mouse
/  3. JoyStick Input
*/

/*
* This class defines the basic API for Input class which needs to be extended
* by the class implementing the 
*
*
*/

class Input
{
private:
	static Input* s_Instance;

protected:
	virtual bool IsKeyDown(int keycode) = 0;
	virtual bool IsKeyUp(int keycode) = 0;
	virtual bool IsKeyRepeated(int keycode) = 0;
	
	virtual bool IsMouseButtonPressed(int button) = 0;
	virtual std::pair<float, float> GetMousePositionImpl() = 0;
	virtual float GetMouseXImpl() = 0;
	virtual float GetMouseYImpl() = 0;



public:

	inline static bool GetKeyDown(int keycode) { return s_Instance->IsKeyDown(keycode); }
	inline static bool GetKeyUp(int keycode) { return s_Instance->IsKeyUp(keycode); }
	inline static bool GetKeyDown(int keycode) { return s_Instance->IsKeyRepeated(keycode); }


	inline static bool GetMouseButton(int button) { return s_Instance->IsMouseButtonPressed(button); }
	inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
	inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
	inline static float GetMouseY() { return s_Instance->GetMouseYImpl();

};


#endif // INPUT_H

	