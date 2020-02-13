#include <Game.h>
#include <iostream>


/**
* Main Entry point to the game.
*/
int main()
{
#if defined(_WIN32)&&(defined(DEBUG) | defined(_DEBUG))
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	//  - You may want to use something more advanced, like Visual Leak Detector
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::cout << "system is currently running thread : " << boost::this_thread::get_id() << "\n";
	gameLoop::Game* game = new gameLoop::Game();

	game->Initialize();
	game->Update();

	delete game;
	return 0;
}
