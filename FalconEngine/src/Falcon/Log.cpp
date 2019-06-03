#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace Falcon {

	LoggerObjects Log::m_loggers;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_loggers.m_EngineLogger = spdlog::stdout_color_mt("FALCON");
		m_loggers.m_EngineLogger->set_level(spdlog::level::trace);
		
		m_loggers.m_GameLogger = spdlog::stdout_color_mt("Game");
		m_loggers.m_GameLogger->set_level(spdlog::level::trace);
	}
}