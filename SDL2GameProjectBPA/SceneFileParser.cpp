#include "SceneFileParser.h"
#include <string>
#include <fstream>

void SceneFileParser::ParseSceneFromFile(const std::string path)
{
	std::ifstream file(path);
	std::string str;
	std::string file_contents;

	if (!file.is_open()) {
		file_contents = "F:" + path;
	}

	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	ParseSceneFromString(file_contents);
}

void SceneFileParser::ParseSceneFromString(const std::string str)
{
}
