#include "pinepch.h"
#include "Logger.h"

namespace Pine {

	std::shared_ptr<spdlog::logger> CoreLogger::s_Logger;

	bool CoreLogger::Initialize()
	{
		CoreLogger::s_Logger = spdlog::stdout_color_mt("CORE");
		CoreLogger::s_Logger->set_level(spdlog::level::debug);

		return true;
	}

	std::shared_ptr<spdlog::logger> AppLogger::s_Logger;
	bool AppLogger::Initialize()
	{
		AppLogger::s_Logger = spdlog::stdout_color_mt("APP");
		AppLogger::s_Logger->set_level(spdlog::level::debug);

		return true;
	}

}