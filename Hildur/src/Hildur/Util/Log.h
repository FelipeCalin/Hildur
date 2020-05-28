#pragma once

#include "Hildur/Core/Core.h"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/fmt/ostr.h>

namespace Hildur {


    struct Message
    {
    public:

        enum class Level : int8_t
        {
            Invalid = -1,
            Trace = 0,
            Debug = 1,
            Info = 2,
            Warn = 3,
            Error = 4,
            Critical = 5,
            Off = 6, // Display nothing
        };

        struct Color { float r, g, b, a; };

    public:

        Message(const std::string message = "", Level level = Level::Invalid);
        inline bool Valid() { return m_Level != Level::Invalid; }
        void OnImGuiRender();

    public:

        const std::string m_Message;
        const Level m_Level;

        static std::unordered_map<Message::Level, Color> s_RenderColors;
    };

	class HILDUR_API Log
	{
	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        inline static std::vector<spdlog::sink_ptr>& GetSinks() { return s_Sinks; }

	private:

        static std::vector<spdlog::sink_ptr> s_Sinks;
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};


}

//Core log macros

#define HR_CORE_TRACE(...)    ::Hildur::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HR_CORE_INFO(...)     ::Hildur::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HR_CORE_WARN(...)     ::Hildur::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HR_CORE_ERROR(...)    ::Hildur::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HR_CORE_CRITICAL(...) ::Hildur::Log::GetCoreLogger()->critical(__VA_ARGS__)

//App log macros

#define HR_TRACE(...)         ::Hildur::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HR_INFO(...)          ::Hildur::Log::GetClientLogger()->info(__VA_ARGS__)
#define HR_WARN(...)          ::Hildur::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HR_ERROR(...)         ::Hildur::Log::GetClientLogger()->error(__VA_ARGS__)
#define HR_CRITICAL(...)      ::Hildur::Log::GetClientLogger()->critical(__VA_ARGS__)
