#pragma once

#include <string>


namespace Hildur {


	class Filepath {

	public:
		
		Filepath() {}
		Filepath(const std::string& filePath);

		std::string getAbsolutePath();
		std::string getExtension();
		std::string getName();
		std::string getPath();
	
	private:
	
		std::string _absolutePath;
		std::string _path;
		std::string _name;
		std::string _extension;
	
	};


}