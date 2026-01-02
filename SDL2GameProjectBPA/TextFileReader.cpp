#include "TextFileReader.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

const char* TextFileReader::ReadText()
{
    if (!limit_once)
    {
       // std::string path = "../Assets/" + std::string("Read.txt");
        std::string path = "Assets/Read.txt";
        std::ifstream file(path);
        file_contents.clear();
        std::string str;
   
        if (!file.is_open()) {
            file_contents = "F:" + path;
            limit_once = true;
            return file_contents.c_str();
        }


        while (std::getline(file, str))
        {
            file_contents += str;
            file_contents.push_back('\n');
        }
        limit_once = true;
        //THIS WORKS, SO THE DATA IS STILL THERE. MY TEXT FILE IS EMPTY? file_contents = std::string("Read.txt");
       
        //Make spreadsheet version
        FileToSpreadsheet();
    }
   
    return file_contents.c_str();
    
}



void TextFileReader::FileToSpreadsheet()
{
    std::vector<DialogueRow> result;

    std::string line;
    std::stringstream ss(file_contents);

    bool firstLine = true;

    while (std::getline(ss, line))
    {
        if (line.empty())
            continue;

        std::stringstream lineStream(line);
        std::string tag, event, dialogue;

        // Split by comma
        std::getline(lineStream, tag, ',');
        std::getline(lineStream, event, ',');
        std::getline(lineStream, dialogue, ',');

        // Skip header row
        if (firstLine)
        {
            firstLine = false;
            continue;
        }

        result.push_back({ tag, event, dialogue });
    }
    StoredRowData = result;


}
const char* TextFileReader::GetStringFromSheetTag(const char* input)
{
    ReadText();
    std::string key = input;
    //LastRequest must never be changed when using the index version of this function. it's text only
    
    // 
    // 
    //should I use size_t instead of int for loops?
    for (int i = 0; i < (int)StoredRowData.size(); ++i)
    {
        //The tag index  is (int) i 
        if (StoredRowData[i].Tag == key)
        {
            if (LastRequest == i)
            {
                //Okay, so we are going to ignore the request if the input is identical.
                //And that's because we are going to read until we hit the end or begin tag!
                return GetStringFromSheetIndex(CurrentLine);
            }
            CurrentLine = i;
            
            LastRequest = i;
            ReplaceBackslashWithNewline(StoredRowData[i].Dialogue);
            return  StoredRowData[i].Dialogue.c_str();
        }
    }
    return "Tag not found!";

}

const char* TextFileReader::GetStringFromSheetIndex(int index)
{
    ReadText();
    CurrentLine = index;
    ReplaceBackslashWithNewline(StoredRowData[index].Dialogue);
    return StoredRowData[index].Dialogue.c_str();
}

int TextFileReader::GetSheetIndexFromString(const char* input)
{
    ReadText();
    std::string key = input;

    //should I use size_t instead of int for loops?
    for (int i = 0; i < (int)StoredRowData.size(); ++i)
    {
        //The tag index  is (int) i 
        if (StoredRowData[i].Tag == key)
        {
            ReplaceBackslashWithNewline(StoredRowData[i].Dialogue);
            return i;
        }
    }
    return 0;
}

bool TextFileReader::HasTagAtIndex(int curIndex, const char* tag)
{
    //PLEAE DON'T DO STRING COMPARISONS!
    //FIND A BETTER WAY!
    return StoredRowData[curIndex].Tag == tag;

}

bool TextFileReader::HasEventAtIndex(int curIndex, const char* tag)
{
    return StoredRowData[curIndex].Event == tag;
}

void TextFileReader::ReplaceBackslashWithNewline(std::string& s)
{
    for (char& c : s)
    {
        if (c == '\\')
            c = '\n';
    }
}
