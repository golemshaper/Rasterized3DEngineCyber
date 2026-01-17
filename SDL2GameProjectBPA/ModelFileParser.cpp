#include "ModelFileParser.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "DrawScratchSpace.h"
#include <map>
#include <fstream>

using namespace std;



Mesh ModelFileParser::ParseFromFile(const std::string path)
{

	std::ifstream file(path);
	std::string str;
	std::string file_contents;

	if (!file.is_open()) {
		file_contents = "F:" + path;
	}

	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	return ParseFromStr(file_contents);
}

Mesh ModelFileParser::ParseFromStr(const std::string str)
{

	std::string sb; 
	sb.reserve(1024); //  boosts performance, allegedly

	//Mode states:
	const int M_Unknown = 0;
	const int M_Vectors = 1;
	const int M_Tris = 2;
	const int M_UVs = 3;
	const int M_ColorPalette = 4;
	const int M_VertColor = 5;
	int Mode = M_Unknown;

	bool Comment_Mode = false;

	//elements to build vectors from
	//Vectors and tris
	std::vector<vec3d> postion_vectors;
	std::vector<vec3d> id_holder_not_real_vectors;
	std::map<int, vec3d > id_vector_map;


	//Color and vertex colors
	std::vector<RGB> color_palette;
	std::map<int, RGB > id_color_map;
	std::vector<vec3d> id_holder_not_real_colors; //color per triangle? 



	
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
			if ((sb) == "vectors")
			{
				Mode = M_Vectors;
			}
			if ((sb) == "tris")
			{
				Mode = M_Tris;
			}
			if ((sb) == "uv")
			{
				Mode = M_UVs;
			}
			if ((sb) == "palette")
			{
				Mode = M_ColorPalette;
			}
			if ((sb) == "color")
			{
				Mode = M_VertColor;
			}
			sb.clear();//clearing data
			continue;
		}
		if (c == '\n')
		{
			sb = trim(sb); //Trim this string on both sides.
			std::vector<std::string>  results = SplitByChar(sb, ' ');
			int colorLineIndex = color_palette.size();
			int lineIndex = postion_vectors.size();
			int triIndex = id_holder_not_real_vectors.size();
			RGB color = RGB{ 0,0,0 };
			std::string data = sb;
			if (results.size() <= 0)continue;
			Comment_Mode = false;
			switch (Mode)
			{
			case M_Vectors:
				
				//we are checking every new line, so this is not all of the vectors, just the 3 elements we have
				//pray trim is enough to get rid of the newline character we probably have.... lol
				
				vec3d vec = vec3d{ std::stof((results[0])), std::stof((results[1])), std::stof((results[2])) };
				postion_vectors.push_back(vec);
				id_vector_map[lineIndex] = vec; //map the ID to a vector for later

				break;
			case M_Tris:
				//Tri data stored in sb. We'll shove the ids in a vec3d
				vec3d vec3d_stored_ids = vec3d{ (float)std::stoi((results[0])), (float)std::stoi((results[1])), (float)std::stoi((results[2])) };
				id_holder_not_real_vectors.push_back(vec3d_stored_ids); //store the IDs so we can get them from the map. We'll cast them to an int later.

				break;
			case M_UVs:
				//UV data stored in sb.
				//do we want a list of Vector2s the same way we have vectors to cut back on UV data?
				//we don't have UVs yet. skip them for now.

				break;
			case M_ColorPalette:
				//build up colors here
				color = RGB{ std::stoi((results[0])), std::stoi((results[1])), std::stoi((results[2])),std::stoi((results[3])) };
				color_palette.push_back(color);
				id_color_map[colorLineIndex] = (color);

				break;
			case M_VertColor:
				//Color data stored in sb.
				//Also make a list of colors and IDs to cut back on color data?
				vec3d color_stored_ids = vec3d{ (float)std::stoi((results[0])), (float)std::stoi((results[1])), (float)std::stoi((results[2])) };
				id_holder_not_real_colors.push_back(color_stored_ids);

				break;
			default:
				break;
			}
			sb.clear();//clearing data
		}

		if (!Comment_Mode)
		{
			sb += c; //continue scanning 
		}
		
		

	}
	Mesh result;

	for (std::size_t i = 0; i < id_holder_not_real_vectors.size(); ++i)
	{
		//construct tri
		int xID = (int)id_holder_not_real_vectors[i].x;
		int yID = (int)id_holder_not_real_vectors[i].y;
		int zID = (int)id_holder_not_real_vectors[i].z;


		vec3d v1 = postion_vectors[xID];
		vec3d v2 = postion_vectors[yID];
		vec3d v3 = postion_vectors[zID];

		triangle tri = triangle{ v1.x, v1.y,  v1.z,   v2.x, v2.y, v2.z,   v3.x,  v3.y, v3.z };
		

		//colors
		int cxID = (int)id_holder_not_real_colors[i].x;
		int cyID = (int)id_holder_not_real_colors[i].y;
		int czID = (int)id_holder_not_real_colors[i].z;

		RGB c1 = color_palette[cxID];
		RGB c2 = color_palette[cyID];
		RGB c3 = color_palette[czID];

		tri.c[0] = c1;
		tri.c[1] = c2;
		tri.c[2] = c3;


		//std::cout << "Vector tri: "<< (int)result.Tris.back().c[0].r << " "<< (int)result.Tris.back().c[0].g << " "<< (int)result.Tris.back().c[0].b << " "<< (int)result.Tris.back().c[0].a << "\n";
		/*std::cout << "vectors: " << id_holder_not_real_vectors.size() << "\n";
		std::cout << "colors:  " << id_holder_not_real_colors.size() << "\n";
		std::cout << "palette: " << color_palette.size() << "\n";
		std::cout << "positions: " << postion_vectors.size() << "\n";*/
		/*if (xID < 0 || xID >= postion_vectors.size() ||
			yID < 0 || yID >= postion_vectors.size() ||
			zID < 0 || zID >= postion_vectors.size()) {

			std::cout << "BAD TRI " << i
				<< " xID=" << xID
				<< " yID=" << yID
				<< " zID=" << zID << "\n";
		}*/
		result.Tris.push_back(tri);
	}

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