#include "hrpcheaders.h"

#include "Hildur/ImGui/ImGuiLogSink.h"

#include <spdlog/sinks/basic_file_sink.h>			// support for basic file logging
#include <spdlog/sinks/rotating_file_sink.h>		// support for rotating file logging
#include <spdlog/sinks/ostream_sink.h>

#include <imgui.h>


namespace Hildur {


	//std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::vector<spdlog::sink_ptr> Log::s_Sinks;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	std::unordered_map<Message::Level, Message::Color> Message::s_RenderColors = {
		{Message::Message::Level::Trace   , {1.0f, 1.0f, 1.0f, 1.0f}}, // White
		{Message::Message::Level::Info    , {0.0f, 1.0f, 0.0f, 1.0f}}, // Green
		{Message::Message::Level::Debug   , {0.0f, 1.0f, 1.0f, 1.0f}}, // Cyan
		{Message::Message::Level::Warn    , {1.0f, 1.0f, 0.0f, 1.0f}}, // Yellow
		{Message::Message::Level::Error   , {1.0f, 0.0f, 0.0f, 1.0f}}, // Red
		{Message::Message::Level::Critical, {1.0f, 0.0f, 1.0f, 1.0f}}  // Magenta
	};

	Message::Message(const std::string message, Hildur::Message::Level level)
		: m_Message(message), m_Level(level)
	{
	}

	void Message::OnImGuiRender()
	{
		if (!Valid()) return;

		Color color = s_RenderColors[m_Level];
		ImGui::PushStyleColor(ImGuiCol_Text, { color.r, color.g, color.b, color.a });
		ImGui::TextUnformatted(m_Message.c_str());
		ImGui::PopStyleColor();
	}

	void Log::Init() 
	{
		//spdlog::set_pattern("%^[%T] %n: %v%$");
		
		//s_CoreLogger = spdlog::stdout_color_mt("HILDUR");
		//s_CoreLogger->set_level(spdlog::level::trace);

		//s_ClientLogger = spdlog::stdout_color_mt("APP");
		//s_ClientLogger->set_level(spdlog::level::trace);

				// create the sinks
		s_Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>()); // VS debug console
		s_Sinks.emplace_back(std::make_shared<ImGuiLogSink_mt>()); // ImGuiConsole

		// create the loggers
		s_CoreLogger = std::make_shared<spdlog::logger>("HAZEL", begin(s_Sinks), end(s_Sinks));
		spdlog::register_logger(s_CoreLogger);
		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(s_Sinks), end(s_Sinks));
		spdlog::register_logger(s_ClientLogger);

		// configure the loggers
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger->set_level(spdlog::level::trace);

		// For testing purpose only
		/*
		for (int i = 0; i < 200; i++)
			s_CoreLogger->trace(i);
		s_CoreLogger->trace("This is a trace message.");
		s_CoreLogger->debug("This is a debug message.");
		s_CoreLogger->info("This is an info message.");
		s_CoreLogger->warn("This is a warning message.");
		s_CoreLogger->error("This is an error message.");
		s_CoreLogger->critical("This is a critical message.");
		s_ClientLogger->trace("This is a trace message.");
		s_ClientLogger->debug("This is a debug message.");
		s_ClientLogger->info("This is an info message.");
		s_ClientLogger->warn("This is a warning message.");
		s_ClientLogger->error("This is an error message.");
		s_ClientLogger->critical("This is a critical message.");
		*/
	}
	

}