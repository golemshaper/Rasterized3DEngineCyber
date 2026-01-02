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

	std::string file_contents; // owns the memory
public:
	std::vector<DialogueRow> StoredRowData; //I CANT BUILD WITH THIS

	int CurrentLine = 0;
	int LastRequest = 1;
	const char* ReadText();
	bool limit_once = false;
	const char* GetStringFromSheetTag(const char* input);
	const char* GetStringFromSheetIndex(int index);
	int GetSheetIndexFromString(const char* input);
	bool HasTagAtIndex(int curIndex, const char* tag);
	bool HasEventAtIndex(int curIndex, const char* tag);
	
	static void ReplaceBackslashWithNewline(std::string& s);
private:
	void FileToSpreadsheet();
};

