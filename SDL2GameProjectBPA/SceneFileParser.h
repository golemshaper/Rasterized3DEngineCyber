#pragma once
#include <string>
#include "DrawScratchSpace.h"
struct SceneObject {
	std::string AssetPath; //The name of the mesh asset. 
	Mesh model; //should this need to be explicitly loaded by the programmer?
	vec3d pos;
	vec3d rot;
	vec3d scale;
	bool visible;
	std::vector<std::string> tags;
};
class SceneFileParser
{
	void ParseSceneFromFile(const std::string path, const std::string mesh_asset_path_root);
	void ParseSceneFromString(const std::string str, const std::string mesh_asset_path_root);
};

