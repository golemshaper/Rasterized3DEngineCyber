#pragma once
#include "Game.h"
class GameTwo : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void RenderMovie();

	void DrawBasics(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	RGB BlurBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
	std::vector<Mesh> MeshSequence;
	int cur_frame = 0;
	float animTimer = 0.0f;
	void AccumulatedBlur(float strength);

	bool screenshot_fire_once = false;

	RGB* Image01;
	RGB* Image02;
	RGB* Image03;

};

