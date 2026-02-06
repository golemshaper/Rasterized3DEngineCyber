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
	std::vector<std::string> tags; //REPLACE THESE WITH TAG IDS!
};
struct TexturePack {
	std::string TextureName;
	RGB* TextureData;
	int width;
	int height;
};
struct DialogueTriggers {
	std::string dialogue;
	int scene_object_ID; //index of the specific scene object...
};

struct Scene {
	std::vector<SceneObject> scene_objects;
	std::vector<TexturePack> TexturePacks;
	std::vector<Mesh> Meshes;

	std::map<std::string, int> tag_map;
	std::map<std::string, int> dialogue_map;
	std::vector<DialogueTriggers> dialogue_triggers;
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
	int GetTagID(const std::string& tag, Scene* scene);


	//DO WE STORE THE CURRENT SCENE INSIDE OF THIS OBJECT?
	//DO WE MAKE THE PROGRAMMER STORE TEXTURE PACKS AND SCENE PACKS THEMSELEVES (AND BY PROGRAMMER, I MEAN ME)
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::vector<std::string> SplitByChars(const std::string& str, char c1, char c2);
	std::string trim(const std::string& s);
	int FindStringIndex(const std::string& target, const std::vector<std::string>& list);


};

