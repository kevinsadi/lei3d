#include "util/Log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace lei3d {
	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %s %v%$");

		s_Logger = spdlog::stdout_color_mt("LEI");
		s_Logger->set_level(spdlog::level::trace);
	}
}