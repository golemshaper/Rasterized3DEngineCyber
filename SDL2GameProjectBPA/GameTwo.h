#pragma once
#include "Game.h"
class GameTwo : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void DrawBasics(float DeltaTime);
	Mesh LoadedMesh;
};

