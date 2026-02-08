#pragma once
#include "Game.h"
#include "miniaudio.h"
#include "SceneFileParser.h"
class ThirdPersonMovement;
class TextSprites;
class TextFileReader;
class GameRPGWorld : public Game
{
public:

	ma_engine audioEngine;   // <— audio playback
	ma_sound music;
	bool loadPositionOnce = true;
	bool MusicLimitOnce = false;
	float fStartMusicTimer = 1.9f;
	bool EnableTextbox = false;

	void Initialize();
	void LoadSceneFiles();
	void LoadSceneFiles(std::string SceneFileName);
	void Tick(float DeltaTime);
	void CollisionProcess(int objId);
	void OldTick(float DeltaTime);
	void MusicAndFadeIn(float DeltaTime);
	void RenderMovie();
	void DrawBasics(float DeltaTime);

	//Scene
	std::string SceneLink = "RPG_WorldMap.txt";
	std::string ScenePath = "Assets/Scenes/";
	std::vector<int> MeshPropIDs;
	SceneFileParser* SceneParser;
	Scene CurrentScene;
	//Scene Tags
	int Tag_Town = -1;
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
	Mesh TerrrainMeshCollider;
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
	RGB* overworldTexture;
	RGB* water;
	RGB* Image03;
	RGB* Palette;

	const char* RequestedText = "RpgIntro";
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text = "";
	void TextUpdateTick(float DeltaTime);
	void TextBoxDraw(const char* input);

	~GameRPGWorld();
};

