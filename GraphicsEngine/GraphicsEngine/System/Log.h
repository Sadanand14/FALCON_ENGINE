#ifndef LOG_H
#define LOG_H

/**
	Disabling warnings thrown out by vendor code
*/
#pragma warning( push )
#pragma warning( disable : 26495)
#pragma warning( disable : 26451)
#pragma warning( disable : 6387)
#pragma warning( disable : 26498)

#include "spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#pragma warning (pop)



#include <memory>



class Log
{

public:
	static void Init();
		
	inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return m_EngineLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetGameLogger()   { return m_GameLogger; }

private:
	static std::shared_ptr<spdlog::logger> m_EngineLogger;
	static std::shared_ptr<spdlog::logger> m_GameLogger;
};

#ifdef BUILD_DEBUG_MODE

	//Engine Logger
	#define FL_ENGINE_INFO(...)  ::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define FL_ENGINE_TRACE(...) ::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define FL_ENGINE_WARN(...)  ::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define FL_ENGINE_ERROR(...) ::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define FL_ENGINE_FATAL(...) ::Log::GetEngineLogger()->fatal(__VA_ARGS__)

	//Game Logger
	#define FL_GAME_INFO(...)    ::Log::GetGameLogger()->info(__VA_ARGS__)
	#define FL_GAME_TRACE(...)   ::Log::GetGameLogger()->trace(__VA_ARGS__)
	#define FL_GAME_WARN(...)    ::Log::GetGameLogger()->warn(__VA_ARGS__)
	#define FL_GAME_ERROR(...)   ::Log::GetGameLogger()->error(__VA_ARGS__)
	#define FL_GAME_FATAL(...)   ::Log::GetGameLogger()->fatal(__VA_ARGS__)

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