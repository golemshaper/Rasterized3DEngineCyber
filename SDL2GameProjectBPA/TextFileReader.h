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
	void ReadText();
	bool limit_once = false;
	std::string Trim(const std::string& s);
	void PrintBytes(const char* label, const std::string& s);
	const char* GetStringFromSheetTag(const char* input);
	const char* GetStringFromSheetIndex(int index);
	int GetSheetIndexFromString(const char* input);
	bool HasTagAtIndex(int curIndex, const char* tag);
	bool HasEventAtIndex(int curIndex, const char* tag);
	bool hasReadFile = false;
	static void ReplaceBackslashWithNewline(std::string& s);
private:
	void FileToSpreadsheet();
};

