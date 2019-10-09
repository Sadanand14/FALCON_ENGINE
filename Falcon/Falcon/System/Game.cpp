#include "Game.h"
#include "Log.h"
#include <iostream>
#include <thread/thread.hpp>
#include "../Core/RenderEvent.h"
#include "../Core/EventManager.h"
//#include "EventManager.h"


int main()
{
	Log::Init();
	WindowClass* window1 = new WindowClass("FalconEngine", 1280, 720);
	InputReceiver* input = new InputReceiver(window1);

	while(!window1->WindowCloseStatus())
	{
		for (unsigned int i = 0; i < 10; i++) {}
			//EventManager::PushEvent(boost::make_shared<RenderEvent>(new RenderEvent), RenderEventCategory);

		window1->Update();
	}

	delete input;
	delete window1;
	return 0;
}
