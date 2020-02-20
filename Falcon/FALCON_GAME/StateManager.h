#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <System/Engine.h>
#include <Types.h>

namespace gameLoop
{
	class GameState
	{
	protected:
		LoopProperties m_properties;
	public:
		virtual void Update() = 0;
		virtual ~GameState() = 0;
	};


	class StateManager
	{

	public:
		static void(*m_update)();
		static void TransitionState(std::string);
	};

}
#endif // !1