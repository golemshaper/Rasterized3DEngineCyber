#pragma once
#include "Game.h"
class GameOne : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
};

