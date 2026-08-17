#pragma once
#include "Game.h"
#include "SceneFileParser.h"
class Animator;
class Scene;
class Mesh;
class GameAthenaRailShmup : public Game
{
public:

	void Initialize();
	void Tick(float DeltaTime);

	void DrawSkyboxMesh();
	void Rail_BKG_Draw(float DeltaTime);
	void DrawBlackBars();
	void LoadScene(std::string SceneFileName);

	std::string SceneLink = "RailCameraEnvironmentThreeMinutes.txt";
	std::string ScenePath = "Assets/Scenes/RailShooter/";

	SceneFileParser* SceneParser;
	Scene SceneParserObject;
	Animator* CameraAnimator;
	Mesh SkyboxMesh;
	vec3d SkyboxLOC;
	RGB* Texture;

	float MouseX=0.0f;
	float MouseY = 0.0f;


	float wait = 3.0f;
	//Tags
	int Tag_Hidden = -1;
	int Tag_SkyboxMesh = -1;
};

