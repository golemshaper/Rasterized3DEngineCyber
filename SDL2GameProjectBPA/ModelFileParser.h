#pragma once

#include "DrawScratchSpace.h"
#include <string>


class ModelFileParser
{
public: 
	Mesh ParseFromStr(const std::string str);
	//vector<std::string> SplitByChar(const std::string& str, char c);
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::string trim(const std::string& s);

	const char* ws = " \t\n\r\f\v";
	

};

