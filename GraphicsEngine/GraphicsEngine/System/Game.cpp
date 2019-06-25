#include "game.h"

#include <iostream>

int main()
{
	Log::Init();
	WindowClass* window1 = new WindowClass("mywindow", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);

	while(!window1->WindowCloseStatus())
	{
		window1->Update();
	}

	delete input;
	delete window1;
	return 0;
}
