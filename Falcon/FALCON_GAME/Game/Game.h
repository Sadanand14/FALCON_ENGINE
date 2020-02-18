#ifndef GAME_H
#define GAME_H

#include <System/Engine.h>
#include <System/Types.h>
#include "GameState.h"

namespace gameLoop
{
	class Game
	{
	private:
		boost::container::vector<GameState> m_states;
		Engine* m_engine;

	public:

		bool Init();
		Game();

		void Update();
		~Game();
	};

}
#endif // !GAME_H
