#pragma once
#include <string>
class TextFileReader
{

	std::string file_contents; // owns the memory
public:
	const char* ReadText();
	bool limit_once = false;
};

