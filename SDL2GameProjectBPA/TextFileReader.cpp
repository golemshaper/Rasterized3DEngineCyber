#include "TextFileReader.h"
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

void TextFileReader::ReadText()
{
    if (!limit_once)
    {
        std::string path = "Assets/Read.txt";
        std::ifstream file(path);
        std::string str;
   
        if (!file.is_open()) {
            file_contents = "F:" + path;
            limit_once = true;
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
        hasReadFile = true;
    }
    
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
std::string TextFileReader::Trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        ++start;

    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        --end;

    return s.substr(start, end - start);
}
void TextFileReader::PrintBytes(const char* label, const std::string& s)
{
    printf("%s (size=%zu): ", label, s.size());
    for (unsigned char c : s)
        printf("[%u]", c);
    printf("\n");
}
const char* TextFileReader::GetStringFromSheetTag(const char* input)
{
    ReadText();
   // std::string key = (input);


   /* PrintBytes("Input inside GetStringFromSheetTag", input);
    printf("Input inside GetStringFromSheetTag: |%s|\n", input);*/


    if (!hasReadFile) {
        return "Tag not found! (File not found)";
    }
    //LastRequest must never be changed when using the index version of this function. it's text only
    
    // 
    // 
    //should I use size_t instead of int for loops?
    for (int i = 0; i < (int)StoredRowData.size(); ++i)
    {
        //The tag index  is (int) i 
        if (strcmp(StoredRowData[i].Tag.c_str(), input) == 0)
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
    if (!hasReadFile) {
        return "Tag not found! (File not found)";
    }
    ReadText();
    CurrentLine = index;
    ReplaceBackslashWithNewline(StoredRowData[index].Dialogue);
    return StoredRowData[index].Dialogue.c_str();
}

int TextFileReader::GetSheetIndexFromString(const char* input)
{
    ReadText();
    std::string key = input;
    if (!hasReadFile) {
        return 0;
    }
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
    if (!hasReadFile)
    {
        return false;
    }
    //PLEAE DON'T DO STRING COMPARISONS!
    //FIND A BETTER WAY!
    return StoredRowData[curIndex].Tag == tag;

}

bool TextFileReader::HasEventAtIndex(int curIndex, const char* tag)
{
    if (!hasReadFile)
    {
        return false;
    }
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
