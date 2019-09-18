#include "Game.h"
#include "Log.h"
#include <iostream>
#include <Memory/fmemory.h>

class Test
{
public:
	Test()
	{
		FL_ENGINE_INFO("using default constor");
	}
	Test(int a)
	{
		FL_ENGINE_INFO("Using Test constructor with 1 argument");
	}
};

int main()
{
	Log::Init();
	fmemory::MemoryManagerInit();

	WindowClass* window1 = new WindowClass("FalconEngine", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);
	Test* test = fnew<Test>(10);

	while(!window1->WindowCloseStatus())
	{
		window1->Update();
	}

	delete input;
	fdelete<WindowClass>(window1);
	fmemory::MeoryManagerShutDown();
	return 0;
}
