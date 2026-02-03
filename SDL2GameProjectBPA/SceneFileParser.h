#pragma once
#include <string>
#include "DrawScratchSpace.h"
#include <map>
struct SceneObject {
	std::string AssetPath; //The name of the mesh asset. 
	int model_id; //should this need to be explicitly loaded by the programmer?
	int texture_id; //id of the texture map
	vec3d pos;
	vec3d rot;
	vec3d scale;
	bool visible;
	std::vector<std::string> tags;
};
struct TexturePack {
	std::string Texture;
	int width;
	int height;
};
struct Scene {
	std::vector<SceneObject> scene_objects;
	std::map<int, TexturePack > texture_pack_map;
};
struct DialogueTriggers {
	std::string dialogue;
	int model_id;
};

class SceneFileParser
{
	void ParseSceneFromFile(const std::string path, const std::string mesh_asset_path_root);
	void ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root);
	int GetModelID(const std::string str);
	int GetTextureId(const std::string str);
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::string trim(const std::string& s);


};

