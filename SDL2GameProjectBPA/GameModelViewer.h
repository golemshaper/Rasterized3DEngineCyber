#pragma once
#include "Game.h"
class GameModelViewer :public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void DrawLaura(float DeltaTime);
	void DrawModelUsingBasicPalette(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	RGB* LauraTexture;
	RGB* BasicPalette;
};

