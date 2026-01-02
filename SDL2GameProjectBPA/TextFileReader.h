#pragma once
#include <string>
#include <vector> 

class TextFileReader
{
	struct DialogueRow
	{
		std::string Tag;
		std::string Event;
		std::string Dialogue;
	};
	std::vector<DialogueRow> StoredRowData; //I CANT BUILD WITH THIS

	std::string file_contents; // owns the memory
public:
	const char* ReadText();
	bool limit_once = false;
	const char* GetStringFromSheet(const char* input);
	
	static void ReplaceBackslashWithNewline(std::string& s);
private:
	void FileToSpreadsheet();
};

