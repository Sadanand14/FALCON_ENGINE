#include "Game.h"
#include "Log.h"
#include <iostream>
#include <thread/thread.hpp>
#include <Core/RenderEvent.h>
#include <Core/EventManager.h>
#include <Memory/fmemory.h>


int main()
{
	Log::Init();

	fmemory::StackAllocator mystack;

	void* ptr = mystack.GetMemoryBlock(100);
	/*fmemory::MemoryManagerInit();

	WindowClass* window1 = fmemory::fnew<WindowClass>("FalconEngine", 1280, 720);
	InputReceiver* input = fmemory::fnew<InputReceiver>(window1);

	std::cout << "system is currently running thread : " << boost::this_thread::get_id() << "\n";


	while(!window1->WindowCloseStatus())
	{
		window1->Update();
		//std::cout<<threadpool->GetSize()<<"\n";
	}

	fmemory::fdelete<InputReceiver>(input);
	fmemory::fdelete<WindowClass> (window1);
	fmemory::MeoryManagerShutDown();*/
	return 0;
}
 