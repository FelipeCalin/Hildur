#include "hrpcheaders.h"

#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging
#include "spdlog/sinks/ostream_sink.h"


namespace Hildur {


	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		//s_CoreLogger = spdlog::stdout_color_mt("HILDUR");
		s_CoreLogger = spdlog::rotating_logger_mt("Hildur", "testLog.log", 1024 * 1024 * 5, 3);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}


}