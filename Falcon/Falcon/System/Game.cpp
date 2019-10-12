#include "Game.h"
#include "Log.h"
#include <iostream>
#include <Memory/fmemory.h>


int main()
{
	Log::Init();

	fmemory::StackAllocator mystack;
	/*fmemory::MemoryManagerInit();

	WindowClass* window1 = fmemory::fnew<WindowClass>("FalconEngine", 1280, 720);
	InputReceiver* input = fmemory::fnew<InputReceiver>(window1);


	while(!window1->WindowCloseStatus())
	{
		window1->Update();
	}

	fmemory::fdelete<InputReceiver>(input);
	fmemory::fdelete<WindowClass> (window1);
	fmemory::MeoryManagerShutDown();*/
	return 0;
}
 