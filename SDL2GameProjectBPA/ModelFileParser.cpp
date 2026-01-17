#include "ModelFileParser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "DrawScratchSpace.h"

using namespace std;



Mesh ModelFileParser::ParseFromStr(const std::string str)
{
	//vectors (store all unique as index based IDs)
	//tris
	//uvs
	//vert colors

	std::string sb; 
	sb.reserve(1024); //  boosts performance, allegedly

	//Mode states:
	const int M_Unknown = 0;
	const int M_Vectors = 1;
	const int M_Tris = 2;
	const int M_UVs = 3;
	const int M_VertColor = 4;
	int Mode = M_Unknown;

	bool Comment_Mode = false;

	for (std::size_t i = 0; i < str.size(); ++i)
	{
		char c = str[i];
		//sb += std::to_string(42);
		//sb.clear();//clear
		//sb += c;//append char
		if (c == '#')
		{
			Comment_Mode = true;
		}

		if (c == ':' && Comment_Mode == false)
		{
			//a new section was marked in the file.
			//such as "positions:" so find out what the name was by reading sb
			if (sb.c_str() == "vectors")
			{
				Mode = M_Vectors;
			}
			if (sb.c_str() == "tris")
			{
				Mode = M_Tris;
			}
			if (sb.c_str() == "uv")
			{
				Mode = M_UVs;
			}
			if (sb.c_str() == "color")
			{
				Mode = M_VertColor;
			}
			sb.clear();//clearing data
			continue; //do not append the token to the data, we are switching modes, and 
		}
		if (c == '\n')
		{
			sb = trim(sb);
			std::vector<std::string>  results = SplitByChar(sb, ' ');

			Comment_Mode = false;
			//new line, so the data we have is going to be of the type of the mode we are in.
			switch (Mode)
			{
			case M_Vectors:
				//Vector data stored in sb.
				//Trim this string on both sides.
				//-0.5 - 0.5  0.5
				for (size_t j = 0; j < results.size(); ++j)
				{
					//todo parse all 3 parts of the data and build up a vector, and put it in a list of vectors
					//later we will build a model from this data
				}

				break;
			case M_Tris:
				//Tri data stored in sb.

				break;
			case M_UVs:
				//UV data stored in sb.

				break;
			case M_VertColor:
				//Color data stored in sb.

				break;
			default:
				break;
			}
		}
	SKIP:
		if (!Comment_Mode)
		{
			sb += c; //continue scanning 
		}
		
		

	}
	Mesh result;
	//Todo construct a mesh
	return result;
}
std::vector<std::string> ModelFileParser::SplitByChar(const std::string& str, char c)
{
	std::vector<std::string> strings;
	std::istringstream f(str);
	std::string s;
	while (std::getline(f, s, c)) {
		strings.push_back(s);
	}
	return strings;
}

std::string ModelFileParser::trim(const std::string& s)
{
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
		++start;

	size_t end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;

	return s.substr(start, end - start);
}