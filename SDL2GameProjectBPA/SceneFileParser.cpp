#pragma once
#include "SceneFileParser.h"
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include "ModelFileParser.h"
#include <iostream>
#include "BMPReader.hpp"



Scene SceneFileParser::ParseSceneFromFile(const std::string scene_file_path, const std::string mesh_asset_path_root)
{
	return ParseSceneFromFile(scene_file_path, mesh_asset_path_root, mesh_asset_path_root);
}

Scene SceneFileParser::ParseSceneFromFile(const std::string scene_file_path, const std::string mesh_asset_path_root, const std::string texture_asset_path_root)
{
	std::ifstream file(scene_file_path);
	std::string str;
	std::string file_contents;

	if (!file.is_open()) {
		file_contents = "F:" + scene_file_path;
	}

	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	//std::cout << "loaded file\n";
	return ParseSceneFromString(file_contents, mesh_asset_path_root);

}

Scene SceneFileParser::ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root)
{
	return ParseSceneFromString(str,mesh_asset_path_root, mesh_asset_path_root);
}

Scene SceneFileParser::ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root, const std::string texture_asset_path_root)
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
	Scene ResultingSceneFile;
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

	//RAW DATA --------------------------------------------
	std::vector<SceneObject> scene_objects;
	std::vector<std::string> model_names;
	std::vector<std::string> texture_names;
	int CurrentModelID = 0;

	std::vector<TexturePack> TexturePacks;
	std::vector<std::string> texture_pack_names;
	int CurrentTextureID = 0;
	//--------------------------------------------

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
				//low priority right now.
				Mode = M_Dialogue;
			}
			sb.clear();//clearing data
			continue;
		}
		if (c == '\n')
		{
			sb = trim(sb); //Trim this string on both sides.
			std::vector<std::string>  results = SplitByChars(sb, ' ',',');
			if (results.size() <= 0)continue;
			if (results.size() <= 1)continue;
			int lineIndex_pos = postion_vectors.size();
			int lineIndex_rot = rotation_vectors.size();
			int lineIndex_scl = scale_vectors.size();


			int posIndex = 0;
			int rotIndex = 0;
			int sclIndex = 0;
			int model_id_holder = 0;
			int texture_id_holder = 0;
			std::string assetName;
			std::string textureName;
			std::vector<std::string> tags;

			int model_index = 0;
			int texture_index = 0;
			bool working_visible = true;

			Comment_Mode = false; //comments have ended
			switch (Mode)
			{
				case M_Positions:
					//Build positions
					working_vec = vec3d{ std::stof((results[0])), -std::stof((results[1])), std::stof((results[2])) };
					postion_vectors.push_back(working_vec);
					id_positions_map[lineIndex_pos] = working_vec;
				break;
				case M_Rotations:
					//Build rotations
					working_vec = vec3d{ std::stof((results[0])), std::stof((results[1])) /360.0f, std::stof((results[2])) };
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
					
					//Grab data:
					assetName = results[0];
					textureName = results[1];
					//Build model list:
					model_index = FindStringIndex(assetName, model_names);
					if (model_index == -1)
					{
						model_names.push_back(assetName);
						model_index = model_names.size()-1;
					}
					//Build texture list:
					texture_index = FindStringIndex(textureName, texture_names);
					if (texture_index == -1 && textureName != "None")
					{
						texture_names.push_back(textureName);
						//consider parsing texture size data from the scene data in blender somehow...
						texture_index = texture_names.size() - 1;
					}

					posIndex = std::stoi(results[2]);
					rotIndex = std::stoi(results[3]);
					sclIndex = std::stoi(results[4]);
					working_visible = (results[5] == "true"); //5 is the index for the visible bool! If it changes, make a constant for it.
					for (int i = 6; i < results.size(); ++i) //6 is the start of the tags. if we add or remove any properties, change this and make a const for it.
					{
						tags.push_back(results[i]);
					}

					working_scene_object = {
						assetName,
						textureName,
						model_index, 
						texture_index,
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
				//do this later...
				//break;
			}
			sb.clear();//clearing data
		}

		if (!Comment_Mode)
		{
			sb += c; //continue scanning 
		}

	}
	//I MAY WANT TO LOOP THROUGH ALL SCENE OBJECTS, AND PUT THE REAL TEXTURE AND MODEL DATA INSIDE BASED ON THE FINAL DATA IN THE SCENE FILE!
	//todo assign Texture and Model data to ResultingSceneFile...
	ModelFileParser model_loader;
	for (int i = 0; i < model_names.size(); i++)
	{
		Mesh cMesh = model_loader.ParseModelFromFile(mesh_asset_path_root + model_names[i]+".txt");
		ResultingSceneFile.Meshes.push_back(cMesh);
		
	}
	
	for (int i = 0; i < texture_names.size(); i++)
	{
		if (texture_names[i] == "None")continue;
		//find a way to get the real w and h from the texture...
		//This is annoying to do, please make a version of the function that takes the string for you, and does this conversion later...
		std::string full = texture_asset_path_root + texture_names[i] + ".bmp";
		const char* tex_path = full.c_str();
		int w = 32; 
		int h = 32;
		RGB* texture = ReadBMP(tex_path, w, h);
		//printf("CALLER: w=%d h=%d &w=%p &h=%p\n", w, h, (void*)&w, (void*)&h);

		TexturePack CurTex = TexturePack{texture_names[i], texture,w,h };
		TexturePacks.push_back(CurTex);

	}
	ResultingSceneFile.TexturePacks = TexturePacks;
	ResultingSceneFile.scene_objects = scene_objects;


	return ResultingSceneFile;
}
int SceneFileParser::GetModelID(const std::string& str, Scene* scene)
{
	//DO WE STORE THE CURRENT SCENE INSIDE OF THIS OBJECT?
	//DO WE MAKE THE PROGRAMMER STORE TEXTURE PACKS AND SCENE PACKS THEMSELEVES (AND BY PROGRAMMER, I MEAN ME)
	//We need to store model IDs and persist this data. 
	return -1;
}

int SceneFileParser::GetModelID(const std::string& str)
{
	return -1;
}

int SceneFileParser::GetTexturePackId(const std::string& str, Scene* scene)
{
	return -1;
}

int SceneFileParser::GetTexturePackId(const std::string& str)
{
	return -1;
}

//int SceneFileParser::GetTagID(const std::string& tag, Scene* scene)
//{
//	auto it = scene->tag_map.find(tag);
//	if (it != scene->tag_map.end())
//		return it->second;
//
//	int newID = scene->tag_map.size();
//	scene->tag_map[tag] = newID;
//	return newID;
//}
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
std::vector<std::string> SceneFileParser::SplitByChars(const std::string& str,char c1, char c2)
{
	std::vector<std::string> out;
	std::string current;

	for (char ch : str)
	{
		if (ch == c1 || ch == c2)
		{
			out.push_back(current);
			current.clear();
		}
		else
		{
			current += ch;
		}
	}

	out.push_back(current);
	return out;
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

int SceneFileParser::FindStringIndex(const std::string& target, const std::vector<std::string>& list)
{
	for (int i = 0; i < (int)list.size(); ++i)
	{
		if (list[i] == target)
			return i;
	}
	return -1; // not found

}
