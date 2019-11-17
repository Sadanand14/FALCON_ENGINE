#include "Game.h"

#include <iostream>


int main()
{
	std::cout << "system is currently running thread : " << boost::this_thread::get_id() << "\n";
	Game* game = new Game();

	if (game->Initialize())
	{
		while (!game->m_windowClosed && !game->m_gameCrashed) 
		{
			game->Update();
		}

		delete game;
	}
	else 
	{
		std::cout << "Something went wrong!" << std::endl;
	}
	return 0;
}
