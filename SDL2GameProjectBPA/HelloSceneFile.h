#pragma once
#include "Game.h"
#include "SceneFileParser.h";

class HelloSceneFile :public Game
{
public:
	void Initialize();
	void LoadSceneFiles();
	void Tick(float DeltaTime);
	bool reload_scene_limit_once = false;
	SceneFileParser* SceneParser;
	Scene SceneParserObject;
};

