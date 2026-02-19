#pragma once
#include "Game.h"
#include "SceneFileParser.h"
#include "StateMachine.h"

class TextSprites;
class TextFileReader;

class GameRPGBattleMode : public Game
{
	TextFileReader* TextFileReaderRef;
public:
	void Initialize();
	void Setup(DrawScratchSpace* scratchspace, TextFileReader* txt, TextSprites* txt_sprites);
	void StartBattle();
	void LoadScene(std::string SceneFileName);
	SceneFileParser* SceneParser;
	Scene SceneParserObject;
	int Tag_Hidden = -1;
	
	std::string SceneLink = "RPG_BattleMode.txt";
	std::string ScenePath = "Assets/Scenes/";
	vec3d CameraStart;
	vec3d CameraEnd;
	float FadeIn = 255;

	void Tick(float DeltaTime);
	bool battleFinished = false;

	int curPartySP = 0;
	int PartySpawnPoints[4];
	int curEnemySP = 0;
	int EnemySpawnPoints[4];

	RGB* Palette;
	int w64 = 64;
	int h64 = 64;

	struct BattleAgent
	{
		bool inParty = false;
		bool alive = false;
		std::string name = "Unknown";

		vec3d loc = vec3d{ 0,0,0 };
		vec3d rot = vec3d{ 0,0,0 };
		vec3d scale = vec3d{ 2.5f,2.5f,2.5f };

		//Stats here?

		//-----------
		Mesh Idle;
		Mesh Attack;
		Mesh Dead;
		Mesh Victory;
	};
	const float PartyMemberScale = 2.5f;
	const static int PartySize = 4;
	BattleAgent PlayerParty[PartySize];
	BattleAgent EnemyParty[PartySize];

};

