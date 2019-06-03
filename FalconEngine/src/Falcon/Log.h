#ifndef LOG_H
#define LOG_H

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"


namespace Falcon {

	/*
	To fix the warning C4251 which is due to spdlog classes are not being dll exported.
	To fix this easiest hack is just create a struct of those objects and use that struct as member is class which is part of  dll.

	https://web.archive.org/web/20170811142318/http://www.microsoft-questions.com/microsoft/VC-Language/30952961/a-solution-to-warning-c4251--class-needs-to-have-dllinterface.aspx
	*/
	struct LoggerObjects
	{
		std::shared_ptr<spdlog::logger> m_EngineLogger;
		std::shared_ptr<spdlog::logger> m_GameLogger;
	};

	class FALCON_API Log
	{

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return m_loggers.m_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetGameLogger()   { return m_loggers.m_GameLogger; }

	private:
		static LoggerObjects m_loggers;
	};
}

#ifdef BUILD_DEBUG_MODE

	//Engine Logger
	#define FL_ENGINE_INFO(...)  ::Falcon::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define FL_ENGINE_TRACE(...) ::Falcon::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define FL_ENGINE_WARN(...)  ::Falcon::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define FL_ENGINE_ERROR(...) ::Falcon::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define FL_ENGINE_FATAL(...) ::Falcon::Log::GetEngineLogger()->fatal(__VA_ARGS__)

	//Game Logger
	#define FL_GAME_INFO(...)    ::Falcon::Log::GetGameLogger()->info(__VA_ARGS__)
	#define FL_GAME_TRACE(...)   ::Falcon::Log::GetGameLogger()->trace(__VA_ARGS__)
	#define FL_GAME_WARN(...)    ::Falcon::Log::GetGameLogger()->warn(__VA_ARGS__)
	#define FL_GAME_ERROR(...)   ::Falcon::Log::GetGameLogger()->error(__VA_ARGS__)
	#define FL_GAME_FATAL(...)   ::Falcon::Log::GetGameLogger()->fatal(__VA_ARGS__)

#else
	#define FL_ENGINE_INFO(...)  
	#define FL_ENGINE_TRACE(...) 
	#define FL_ENGINE_WARN(...)  
	#define FL_ENGINE_ERROR(...) 
	#define FL_ENGINE_FATAL(...) 

	#define FL_GAME_INFO(...)    
	#define FL_GAME_TRACE(...)   
	#define FL_GAME_WARN(...)    
	#define FL_GAME_ERROR(...)   
	#define FL_GAME_FATAL(...)   

#endif
#endif //LOG_H