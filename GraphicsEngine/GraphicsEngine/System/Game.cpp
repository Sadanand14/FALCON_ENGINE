#include "Game.h"
#include "Application.h"


int main()
{

	#ifdef FL_DEBUG_BUILD
		// Enable memory leak detection as a quick and dirty
		// way of determining if we forgot to clean something up
		//  - You may want to use something more advanced, like Visual Leak Detector
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	auto application = new Application();
	application->Run();
	return 0;
}
