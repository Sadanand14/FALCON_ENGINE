#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <System/Engine.h>

namespace gameLoop
{
	class GameState
	{
	protected:
		LoopProperties m_properties;
	public:		
		virtual void Update(){}
		virtual ~GameState() {};
	};


	class IngameState : public GameState 
	{
	public:
		IngameState() 
		{};

		virtual Update() override {}
		virtual ~IngameState() {}
	};
}

#endif // !1