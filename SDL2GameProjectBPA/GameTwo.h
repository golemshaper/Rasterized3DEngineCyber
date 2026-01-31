#pragma once
#include "Game.h"
class ThirdPersonMovement;
class GameTwo : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void RenderMovie();

	void DrawBasics(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	Mesh PlayerMesh;
	Mesh TerrrainMesh;
	Mesh WaterPlaneMesh;
	ThirdPersonMovement* PlayerMovement;
	vec3d CameraSmoothLocation = vec3d{ 0.0f, 12.9f, -17.5f };
	vec3d CameraSmoothRotation = vec3d{ 0.0f, 8.0f, 17.5f };
	RGB BlurBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
	std::vector<Mesh> MeshSequence;
	int cur_frame = 0;
	float animTimer = 0.0f;
	void AccumulatedBlur(float strength);

	bool screenshot_fire_once = false;
	RGB GI_Lighting = {255,255,255,255};
	//textures
	RGB* Image01;
	RGB* grass;
	RGB* water;
	RGB* Image03;
	RGB* Palette;
	~GameTwo();
};

