#pragma once
#include <string>
#include "DrawScratchSpace.h"
#include <map>
struct SceneObject {
	std::string AssetPath; //The name of the mesh asset. 
	std::string TextureAssetPath; //The name of the mesh asset. 
	int model_id; //should this need to be explicitly loaded by the programmer?
	int texture_id; //id of the texture map
	vec3d pos;
	vec3d rot;
	vec3d scale;
	bool visible;
	std::vector<std::string> tag_strings; //REPLACE THESE WITH TAG IDS!
	std::vector<int> tag_ids;
	RGB color = RGB_White;
	/// <summary>
	/// String comparisons are to slow for a game. Make an ID system!
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	bool HasTagStringCompare(const std::string& tag)
	{
		for (int i = 0; i < tag_strings.size(); ++i)
		{
			if (tag_strings[i] == tag)return true;
		}
		return false;
	}
	bool HasTagIDOutIndex(int tag_id, int& index)
	{
		for (int i = 0; i < tag_ids.size(); ++i)
		{
			if (tag_ids[i] == tag_id)
			{
				index = i;
				return true;
			}
		}
		index = -1;
		return false;
	}
	bool HasTagByID(int tag_id)
	{
		for (int i = 0; i < tag_ids.size(); ++i)
		{
			if (tag_ids[i] == tag_id)
			{
				return true;
			}
		}
		return false;
	}
};
struct TexturePack {
	std::string TextureName;
	RGB* TextureData;
	int width;
	int height;
};
struct DialogueTriggers {
	//not in use currently
	std::string dialogue;
	int scene_object_ID; //index of the specific scene object...
};

struct Scene {
	std::vector<SceneObject> scene_objects;
	std::vector<TexturePack> TexturePacks;
	std::vector<Mesh> Meshes;
	std::vector<std::string> TagStrList;
	
	int GetTagID(std::string tag) {

		for (int i = 0; i < TagStrList.size(); ++i)
		{
			if (TagStrList[i] == tag)return i;
		}
		return -1;
	}
	std::string GetTagStringFromID(int tag_id)
	{
		return TagStrList[tag_id];
	}
	//UNUSED:
	std::map<std::string, int> dialogue_map;
	std::vector<DialogueTriggers> dialogue_triggers;

	/// <summary>
	/// Returns the tag to the right of the given tag id.
	/// </summary>
	/// <param name="obj"></param>
	/// <param name="tag_id"></param>
	/// <param name="out_arg"></param>
	/// <returns></returns>
	bool GetTagArgument(const SceneObject& obj, int tag_id, std::string& out_arg) const;
	bool GetTagArgument(const SceneObject& obj, int tag_id, std::string& out_arg,int arg_Number) const;
	bool GetTagArgument(int objId, int tag_id, std::string& out_arg) const;
	bool GetTagArgument(int objId, int tag_id, std::string& out_arg, int arg_number) const;
	
};

class SceneFileParser
{
public:
	Scene ParseSceneFromFile(const std::string scene_file_path, const std::string mesh_asset_path_root);
	Scene ParseSceneFromFile(const std::string scene_file_path, const std::string mesh_asset_path_root, const std::string texture_asset_path_root);
	
	Scene ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root);
	Scene ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root, const std::string texture_asset_path_root);
	int GetModelID(const std::string& str, Scene* scene);
	int GetModelID(const std::string& str); //Instead of passing a scene, should this just be a function of Scene?
	int GetTexturePackId(const std::string& str, Scene* scene);
	int GetTexturePackId(const std::string& str);
	//int GetTagID(const std::string& tag, Scene* scene);


	//DO WE STORE THE CURRENT SCENE INSIDE OF THIS OBJECT?
	//DO WE MAKE THE PROGRAMMER STORE TEXTURE PACKS AND SCENE PACKS THEMSELEVES (AND BY PROGRAMMER, I MEAN ME)
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::vector<std::string> SplitByChars(const std::string& str, char c1, char c2);
	std::string trim(const std::string& s);
	int FindStringIndex(const std::string& target, const std::vector<std::string>& list);


};

