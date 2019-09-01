#include "Game.h"
#include "Log.h"
#include <iostream>

int main()
{
	Log::Init();
	Window* window1 = new Window("FalconEngine", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);

	while(!window1->GetWindowShouldClose())
	{
		window1->Update();
	}

	delete input;
	delete window1;
	return 0;
}
