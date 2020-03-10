#pragma once

#include "Hildur/Core/Core.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

namespace Hildur {

	class HILDUR_API Log {

	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

//Core log macros

#define HR_CORE_TRACE(...)   ::Hildur::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HR_CORE_INFO(...)    ::Hildur::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HR_CORE_WARN(...)    ::Hildur::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HR_CORE_ERROR(...)   ::Hildur::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HR_CORE_FATAL(...)   ::Hildur::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//App log macros

#define HR_TRACE(...)        ::Hildur::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HR_INFO(...)         ::Hildur::Log::GetClientLogger()->info(__VA_ARGS__)
#define HR_WARN(...)         ::Hildur::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HR_ERROR(...)        ::Hildur::Log::GetClientLogger()->error(__VA_ARGS__)
#define HR_FATAL(...)        ::Hildur::Log::GetClientLogger()->fatal(__VA_ARGS__)
