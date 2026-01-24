#pragma once
#include "Game.h"
class GameModelViewer :public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	Mesh LoadedMesh;
};

