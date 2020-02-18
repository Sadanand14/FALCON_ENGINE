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
		std::string m_title;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual ~GameState() = 0;
	};


	class IngameState : public GameState 
	{
	public:
		IngameState() 
		{};

		virtual void Start() override {}
		virtual void Update() override {}
		virtual ~IngameState() {}
	};
}

#endif // !1