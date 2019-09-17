#include "Game.h"
#include "Log.h"
#include <iostream>
//#include "EventManager.h"

int main()
{
	Log::Init();
	WindowClass* window1 = new WindowClass("FalconEngine", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);

	while(!window1->WindowCloseStatus())
	{
		window1->Update();
	}

	delete input;
	delete window1;
	return 0;
}
