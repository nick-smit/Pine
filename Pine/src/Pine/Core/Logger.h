#pragma once

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include <spdlog\spdlog.h>
#include <spdlog\sinks\stdout_color_sinks.h>
#include <spdlog\fmt\ostr.h>

namespace Pine {

	class CoreLogger {
	public:
		CoreLogger() = delete;
		~CoreLogger() = delete;

	public:
		static bool Initialize();

		static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; };
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

	class AppLogger {
	public:
		AppLogger() = delete;
		~AppLogger() = delete;

	public:
		static bool Initialize();

		static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; };
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

#define PINE_LOG_CORE_DEBUG(...) ::Pine::CoreLogger::GetLogger()->debug(__VA_ARGS__)
#define PINE_LOG_CORE_INFO(...) ::Pine::CoreLogger::GetLogger()->info(__VA_ARGS__)
#define PINE_LOG_CORE_WARN(...) ::Pine::CoreLogger::GetLogger()->warn(__VA_ARGS__)
#define PINE_LOG_CORE_CRITICAL(...) ::Pine::CoreLogger::GetLogger()->critical(__VA_ARGS__)

#define PINE_LOG_DEBUG(...) ::Pine::AppLogger::GetLogger()->debug(__VA_ARGS__)
#define PINE_LOG_INFO(...) ::Pine::AppLogger::GetLogger()->info(__VA_ARGS__)
#define PINE_LOG_WARN(...) ::Pine::AppLogger::GetLogger()->warn(__VA_ARGS__)
#define PINE_LOG_CRITICAL(...) ::Pine::AppLogger::GetLogger()->critical(__VA_ARGS__)
