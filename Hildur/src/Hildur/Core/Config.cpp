#include "hrpcheaders.h"
#include "Config.h"

#include <algorithm>
#include <fstream>
#include <iostream>


namespace Hildur {


	Config::Profile Config::profile;

	void Config::readConfig()
	{
		std::ifstream file(FILE_PATH);
		std::string line;
		std::vector<std::string> tokens;

		if (file.fail())
		{
			HR_CORE_INFO("Creating new config file.");
			createDefault();

			return;
		}

		while (std::getline(file, line))
		{
			tokens.clear();
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			split(line, "=", tokens);

			if (tokens[0] == "" || tokens[0].at(0) == '#')
			{
				continue;
			}
			else if (tokens[0].compare("appName") == 0)
			{
				profile.appName = tokens[1];
			}
			else if (tokens[0].compare("width") == 0)
			{
				profile.width = std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("height") == 0)
			{
				profile.height = std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("fullscreen") == 0)
			{
				profile.fullscreen = (bool)std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("vsync") == 0)
			{
				profile.vsync = (bool)std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("supersampling") == 0)
			{
				profile.supersampling = (bool)std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("show_fps") == 0)
			{
				profile.showFPS = (bool)std::stoi(tokens[1]);
			}
			else if (tokens[0].compare("fps_profiling") == 0)
			{
				profile.fpsProfiling = (bool)std::stoi(tokens[1]);
			}
			else
			{
				HR_CORE_WARN("Unknown token: {0}", tokens[0]);
			}
		}

		file.close();
	}

	void Config::createDefault()
	{
		std::string def = "#Hildur Engine Configuration file\n"
			"appName = PhotonBox\n"
			"width = 1280\n"
			"height = 720\n"
			"fullscreen = 0\n"
			"vsync = 0\n"
			"supersampling = 0\n"
			"show_fps = 0\n"
			"fps_profiling = 0\n";

		std::ofstream file(FILE_PATH);
		file << def;
		file.close();
	}

	void Config::split(const std::string& line, const std::string& delimiter, std::vector<std::string>& output)
	{
		std::string s = line;
		size_t pos = 0;
		std::string token;

		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			output.push_back(token);
			s.erase(0, pos + delimiter.length());
		}

		output.push_back(s);
	}


}