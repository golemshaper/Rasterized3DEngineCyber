#include "SceneFileParser.h"
#include <string>
#include <fstream>

void SceneFileParser::ParseSceneFromFile(const std::string path, const std::string mesh_asset_path_root)
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
	ParseSceneFromString(file_contents, mesh_asset_path_root);
}

void SceneFileParser::ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root)
{
	
	/*NEW DRAFT:
	----------------------------------------------------------------------------------------
					Positions:
					0.000 0.000 0.000
					2.000 0.000 1.500
					Rotations:
					0.000 0.000 0.000
					Scales:
					1,1,1

					Models:
					#Model, Texture, Location, Rotation, Scale ID, visibility, Misc Tags
					Human,BasicPaletteTexture,1,0,0,true,Tag1,Tag2

					DialogueTriggers:
					"WelcomeToTheWorld",model ID
	----------------------------------------------------------------------------------------
	*/

	std::string sb;
	sb.reserve(1024);

	//Mode states:
	const int M_Unknown = 0;
	const int M_Positions = 1;
	const int M_Rotations = 2;
	const int M_Scales = 3;
	int Mode = M_Unknown;
	bool Comment_Mode = false;

	for (std::size_t i = 0; i < str.size(); ++i)
	{
		char c = str[i];

		if (c == '#')
		{
			Comment_Mode = true;
		}
		if (c == ':' && Comment_Mode == false)
		{
			//a new section was marked in the file via a ':' .
			//such as "positions:" so find out what the name was by reading sb
			if ((sb) == "Positions")
			{
				Mode = M_Positions;
			}
			if ((sb) == "Rotations")
			{
				Mode = M_Rotations;
			}
			if ((sb) == "Scales")
			{
				Mode = M_Scales;
			}
		}
		if (c == '\n')
		{
			Comment_Mode = false;
			switch (Mode)
			{
				case M_Positions:
				//Build positions
				break;
				case M_Rotations:
				//Build rotations
				break;
				case M_Scales:
				//Build scales
				break;
			}
		}
	}
}
