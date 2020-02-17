#ifndef GAME_H
#define GAME_H

#include <System/Types.h>
#include "GameState.h"

class Game 
{
private:
	boost::container::vector<GameState> m_states;


public:

	Game();
	~Game();
};

#endif // !GAME_H
