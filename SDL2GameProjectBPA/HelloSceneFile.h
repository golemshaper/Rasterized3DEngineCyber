#pragma once
#include "Game.h"
#include "SceneFileParser.h";

class HelloSceneFile :public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);

	SceneFileParser* SceneParser;
	Scene MyScene;
};

