#pragma once
#include "pch.h"
#include "core.h"

namespace tze {
	class TZE_API Log
	{
	public:
		Log() = default;
		~Log() = default;

		static void init();

		
		inline static std::shared_ptr<spdlog::logger>& getGame_engineLogger() {
			return s_game_engineLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& getClientLogger() {
			return s_clientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_game_engineLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#ifndef CLIENT_MODE

#define TZE_ENGINE_ERR(...)      ::tze::Log::getGame_engineLogger()->error(__VA_ARGS__)
#define TZE_ENGINE_WARN(...)     ::tze::Log::getGame_engineLogger()->warn(__VA_ARGS__)
#define TZE_ENGINE_INFO(...)     ::tze::Log::getGame_engineLogger()->info(__VA_ARGS__)
#define TZE_ENGINE_TRC(...)      ::tze::Log::getGame_engineLogger()->trace(__VA_ARGS__)

#define TZE_CLIENT_ERR(...)      ::tze::Log::getClientLogger()->error(__VA_ARGS__)
#define TZE_CLIENT_WARN(...)     ::tze::Log::getClientLogger()->warn(__VA_ARGS__)
#define TZE_CLIENT_INFO(...)     ::tze::Log::getClientLogger()->info(__VA_ARGS__)
#define TZE_CLIENT_TRC(...)      ::tze::Log::getClientLogger()->trace(__VA_ARGS__)

#else

#define TZE_ENGINE_ERR(...) 
#define TZE_ENGINE_WARN(...)
#define TZE_ENGINE_INFO(...)
#define TZE_ENGINE_TRC(...) 

#define TZE_CLIENT_ERR(...) 
#define TZE_CLIENT_WARN(...)
#define TZE_CLIENT_INFO(...)
#define TZE_CLIENT_TRC(...) 

#endif