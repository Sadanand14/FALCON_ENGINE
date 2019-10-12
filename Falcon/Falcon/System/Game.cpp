#include "Game.h"
#include "Log.h"
#include <iostream>
#include "..//System/ThreadPool.h"
#include <thread/thread.hpp>
#include "../Core/RenderEvent.h"
#include "../Core/EventManager.h"
//#include "EventManager.h"


int main()
{
	Log::Init();
	WindowClass* window1 = new WindowClass("FalconEngine", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);
	
	std::cout << "system is currently running thread : " << std::this_thread::get_id() << "\n";

	while(!window1->WindowCloseStatus())
	{
	

		window1->Update();
		//std::cout<<threadpool->GetSize()<<"\n";
	}

	delete input;
	delete window1;
	return 0;
}
