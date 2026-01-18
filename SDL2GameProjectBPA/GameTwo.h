#pragma once
#include "Game.h"
class GameTwo : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void DrawBasics(float DeltaTime);
	Mesh LoadedMesh;
	RGB BlurBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
	std::vector<Mesh> MeshSequence;
	int cur_frame = 0;
	float animTimer = 0.0f;
	void AccumulatedBlur(float strength);
};

