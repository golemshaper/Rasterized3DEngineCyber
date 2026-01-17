#include "ModelFileParser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "DrawScratchSpace.h"
#include <map>

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

	//elements to build vectors from
	std::vector<vec3d> postion_vectors;
	std::map<int, vec3d > id_vector_map;

	std::vector<int> tris_by_vec_id;


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
			sb = trim(sb); //Trim this string on both sides.
			std::vector<std::string>  results = SplitByChar(sb, ' ');

			Comment_Mode = false;
			switch (Mode)
			{
			case M_Vectors:
				//All vectors in the file are in the vector list.
				
				//-0.5 - 0.5  0.5
				for (size_t j = 0; j < results.size(); j+=3)
				{
					//pray trim is enough to get rid of the newline character we probably have.... lol
					vec3d vec = vec3d{ std::stof(trim(results[j])), std::stof(trim(results[j + 1])), std::stof(trim(results[j + 2])) };
					postion_vectors.push_back(vec);
					id_vector_map[i] = vec;
				}

				break;
			case M_Tris:
				//Tri data stored in sb.
				//will need a map of index to vector. Will also need to store the data in tris-as-is. wee can only create the mesh once the file is finished.

				break;
			case M_UVs:
				//UV data stored in sb.
				//do we want a list of Vector2s the same way we have vectors to cut back on UV data?

				break;
			case M_VertColor:
				//Color data stored in sb.
				//Also make a list of colors and IDs to cut back on color data?

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