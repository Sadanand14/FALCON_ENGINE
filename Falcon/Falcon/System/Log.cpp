#include "Log.h"


std::shared_ptr<spdlog::logger> Log::m_EngineLogger;
std::shared_ptr<spdlog::logger> Log::m_GameLogger;

/*
* Following method initializes logging system.
*/

void Log::Init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	m_EngineLogger = spdlog::stdout_color_mt("FALCON");
	m_EngineLogger->set_level(spdlog::level::trace);
		
	m_GameLogger = spdlog::stdout_color_mt("Game");
	m_GameLogger->set_level(spdlog::level::trace);
}
