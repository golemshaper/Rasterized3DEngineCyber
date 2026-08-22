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
	void Reload();
	
	void DrawSkyboxMesh();
	void Rail_BKG_Draw(float DeltaTime);
	void DrawEnemy(float DeltaTime);
	void DrawSingleEnemyShipRoutine(std::string ShipAnimTag,int sceneObjId);
	void PlayerMovement(float DeltaTime);
	
	void DrawPlayer(float DeltaTime);
	void DrawReticle(float DeltaTime);
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
	int Tag_Player = -1;
	int Tag_Cursor = -1;
	int Tag_SkyboxMesh = -1;

	int Tag_EnemyShip0 = -1;
	int Tag_EnemyShip1 = -1;
	int Tag_EnemyShip2 = -1;
	int Tag_EnemyShip3 = -1;
	int Tag_EnemyShip4 = -1;
	int Tag_EnemyShip5 = -1;

	int SceneID_EnemyShip0 = -1;
	int SceneID_EnemyShip1 = -1;
	int SceneID_EnemyShip2 = -1;
	int SceneID_EnemyShip3 = -1;
	int SceneID_EnemyShip4 = -1;
	int SceneID_EnemyShip5 = -1;

	//Structs:
	struct Player
	{
		bool firstRun = true;
		vec3d local_position;
		vec3d storeInputVector;

		vec3d perceptual_location; //use for collision. has hard offset
		float speed = 2.0f;
		float maxDist = 2.5f;
		vec3d cameraLoc;
		Mesh Mesh_PlayerIdle;
		int CursorTextureId;
		Mesh Mesh_Cursor;
		std::vector<Mesh> AttackMeshSequence;

	};
	Player player;
};

