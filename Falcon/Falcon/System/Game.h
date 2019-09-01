#ifndef game_h
#define game_h

#include "Application.h"

class Game : public Application
{
public:
	Game();
	~Game();
};

Application* CreateApplication()
{
	return new Game();
}
#endif