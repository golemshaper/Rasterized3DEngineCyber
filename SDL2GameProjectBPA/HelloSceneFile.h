#pragma once
#include "Game.h"
#include "SceneFileParser.h";

class HelloSceneFile :public Game
{
public:
	void Initialize();
	void LoadSceneFiles(std::string SceneFileName);
	void LoadSceneFiles();
	void Tick(float DeltaTime);
	bool reload_scene_limit_once = false;
	Mesh TerrainCollider;
	SceneFileParser* SceneParser;
	Scene SceneParserObject;
	vec3d CameraStart;
	vec3d CameraEnd;
	std::string SceneLink = "HELLO_SCENE_FILE.txt";
	std::string ScenePath = "Assets/Scenes/";
	int Tag_Hidden = -1;
	int Tag_Spin= -1;
	int Tag_LevelLink;
	int PlayerID = -1;

};

