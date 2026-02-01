#pragma once
#include "Game.h"
#include "miniaudio.h"
class ThirdPersonMovement;
class TextSprites;
class TextFileReader;
class GameRPGWorld : public Game
{
public:

	ma_engine audioEngine;   // <— audio playback


	void Initialize();
	void Tick(float DeltaTime);
	void RenderMovie();

	void DrawBasics(float DeltaTime);
	//Text
	TextSprites* MyTextSprites;
	TextFileReader* Reader;

	//Mesh
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	//Player
	Mesh PlayerMesh_Idle;
	Mesh PlayerMesh_Walk;
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

	const char* RequestedText = "RpgIntro";
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text = "";
	void TextBoxDraw(const char* input);

	~GameRPGWorld();
};

