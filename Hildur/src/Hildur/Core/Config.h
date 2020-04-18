#pragma once

#include <string>
#include <vector>


namespace Hildur {


	class Config
	{
	public:

		struct Profile
		{
			std::string appName = "Hildur Engine";
			int width = 1280;
			int height = 720;
			bool fullscreen = false;
			bool vsync = false;
			bool supersampling = false;
			bool showFPS = false;
			bool fpsProfiling = false;
		};

		static Profile profile;

		const std::string FILE_PATH = "config.cfg";
		void readConfig();
		void createDefault();

	private:

		void split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output);
	};


}