#include "Game.h"

#include <iostream>


int main()
{
	std::cout << "system is currently running thread : " << boost::this_thread::get_id() << "\n";
	Game* game = new Game();

	game->Initialize();
	game->Update();
	
	delete game;
	return 0;
}
