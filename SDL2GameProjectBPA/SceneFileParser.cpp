#include "SceneFileParser.h"
#include <string>
#include <sstream>
#include <fstream>
#include <map>

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

					Objects:
					#Model, Texture, Location, Rotation, Scale ID, visibility, Misc Tags
					Human,BasicPaletteTexture,1,0,0,true,Tag1,Tag2

					DialogueTriggers:
					"WelcomeToTheWorld",obj ID
	----------------------------------------------------------------------------------------
	*/

	std::string sb;
	sb.reserve(1024);

	//Mode states:
	const int M_Unknown = 0;
	const int M_Positions = 1;
	const int M_Rotations = 2;
	const int M_Scales = 3;
	const int M_Objects = 4;
	const int M_Dialogue= 5;

	int Mode = M_Unknown;
	bool Comment_Mode = false;

	//It seems silly to keep these vectors as different catagories, but it's proably easier to hand edit a scene, should the need arise...
	std::vector<vec3d> postion_vectors;
	std::map<int, vec3d > id_positions_map;

	std::vector<vec3d> rotation_vectors;
	std::map<int, vec3d > id_rotations_map;

	std::vector<vec3d> scale_vectors;
	std::map<int, vec3d > id_scales_map;


	std::vector<SceneObject> scene_objects;

	std::vector<DialogueTriggers> dialogue_triggers;

	vec3d working_vec{ 0,0,0 };
	vec3d working_vec2{ 0,0,0 };
	vec3d working_vec3{0,0,0};
	SceneObject working_scene_object;

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
			if ((sb) == "Objects")
			{
				Mode = M_Objects;
			}
			if ((sb) == "DialogueTriggers")
			{
				Mode = M_Dialogue;
			}
		}
		if (c == '\n')
		{
			sb = trim(sb); //Trim this string on both sides.
			std::vector<std::string>  results = SplitByChar(sb, ' ');
			int lineIndex_pos = postion_vectors.size();
			int lineIndex_rot = rotation_vectors.size();
			int lineIndex_scl = scale_vectors.size();


			int posIndex = 0;
			int rotIndex = 0;
			int sclIndex = 0;
			int model_id_holder = 0;
			int texture_id_holder = 0;
			bool working_visible = true;
			std::string assetName;
			std::string textureName;
			std::vector<std::string> tags;


			Comment_Mode = false; //comments have ended
			switch (Mode)
			{
				case M_Positions:
					//Build positions
					working_vec = vec3d{ std::stof((results[0])), std::stof((results[1])), std::stof((results[2])) };
					postion_vectors.push_back(working_vec);
					id_positions_map[lineIndex_pos] = working_vec;
				break;
				case M_Rotations:
					//Build rotations
					working_vec = vec3d{ std::stof((results[0])), std::stof((results[1])), std::stof((results[2])) };
					rotation_vectors.push_back(working_vec);
					id_rotations_map[lineIndex_rot] = working_vec;

				break;
				case M_Scales:
					//Build scales
					working_vec = vec3d{ std::stof((results[0])), std::stof((results[1])), std::stof((results[2])) };
					scale_vectors.push_back(working_vec);
					id_scales_map[lineIndex_scl] = working_vec;

				break;
				case M_Objects:
					//Build objects
					//ASSUME INPUT LIKE:
					//"Human, BasicPaletteTexture, 1, 0, 0, true, Tag1, Tag2"
					
					assetName = results[0];
					textureName = results[1];

					posIndex = std::stoi(results[2]);
					rotIndex = std::stoi(results[3]);
					sclIndex = std::stoi(results[4]);
					bool visible = (results[5] == "true"); //5 is the index for the visible bool! If it changes, make a constant for it.
					for (int i = 6; i < results.size(); ++i) //6 is the start of the tags. if we add or remove any properties, change this and make a const for it.
					{
						tags.push_back(results[i]);
					}

					working_scene_object = {
						assetName,
						GetModelID(assetName),
						GetTextureId(textureName),
						id_positions_map[posIndex],
						id_rotations_map[rotIndex],
						id_scales_map[sclIndex],
						working_visible,
						tags
					};
					
					scene_objects.push_back(working_scene_object);


				break;
				//case M_Dialogue:
				//	//Build dialogue

				//break;
			}
		}

		if (!Comment_Mode)
		{
			sb += c; //continue scanning 
		}

	}
}
int SceneFileParser::GetModelID(const std::string str)
{
	return 0;
}
int SceneFileParser::GetTextureId(const std::string str)
{
	return 0;
}
std::vector<std::string> SceneFileParser::SplitByChar(const std::string& str, char c)
{
	std::vector<std::string> strings;
	std::istringstream f(str);
	std::string s;
	while (std::getline(f, s, c)) {
		strings.push_back(s);
	}
	return strings;
}

std::string SceneFileParser::trim(const std::string& s)
{
	size_t start = 0;
	while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
		++start;

	size_t end = s.size();
	while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;

	return s.substr(start, end - start);
}