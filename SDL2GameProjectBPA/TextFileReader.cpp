#include "TextFileReader.h"
#include <fstream>
#include <string>

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
    }

    return file_contents.c_str();
    
}
