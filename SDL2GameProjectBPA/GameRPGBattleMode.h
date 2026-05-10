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
	TextFileReader* Reader;
	Scene SceneParserObject;
	int Tag_Hidden = -1;
	
	std::string SceneLink = "RPG_BattleMode.txt";
	std::string ScenePath = "Assets/Scenes/";
	std::string RequestedText = "...";
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

	struct RPGStats
	{
		int HP = 10;
		int MAX_HP = 10;
		int ATK = 1;
		int INT = 1;
		int SPD = 1;

	};
	struct BattleAgent
	{
		bool inParty = false;
		bool alive = false;
		std::string name = "Unknown";

		vec3d loc = vec3d{ 0,0,0 };
		vec3d rot = vec3d{ 0,0,0 };
		vec3d scale = vec3d{ 2.5f,2.5f,2.5f };
		vec3d home_loc = vec3d{ 0,0,0 };
		vec3d target_loc = vec3d{ 0,0,0 };

		float travel = 0.0f;
		//Stats here
		RPGStats Stats;
		//-----------
		Mesh Idle; //set in the overworld for the player at start of game by default I think...
		Mesh Attack;
		Mesh Dead;
		Mesh Victory;
	};
	struct RPGAction
	{
		//Data:
		bool ActionIsQueued = false;
		int actorID = 0;
		int HurtTargetAmount = 0;
		int HealTargetAmount = 0;
		int HurtDefenseAmount = 0;
		int HurtAttackAmount = 0;
		bool TargetAllOnTeam = false;
		//move in, wait, play attack animation, exit back to starting value
		//Animation:
		float moveInTimer;
		float actionTime;
		float moveOutTimer;
	};
	RPGAction RPGActionsList[8];//all actions get queued
	int CurrentRPGAction = 0;

	const float PartyMemberScale = 2.5f;
	const static int PartySize = 4;
	BattleAgent PlayerParty[PartySize];
	BattleAgent EnemyParty[PartySize];
	BattleAgent AllActors[PartySize + PartySize]; //Thinking of combining all actors in to a single AllActors list to make Actions easier to reason about.
	

	void MoveBattleAgentToTarget(int MoveAgentID, BattleAgent(&MoveAgentPool)[PartySize], int AgentTargetID, BattleAgent(&TargetAgentPool)[PartySize], float amount);
	



	void SetEncounterDummyData();

	//BATTLE MENU
	void DrawBattleMenu();
	//Consider: Menu Tick and Menu Draw should be two different things
	//Consider: Have some bit of data in a character to store the command they need to execute
	
	void DrawWindowStartSection();
	void DrawWindowHighlight();
	bool window_draw_start = false;
	bool window_draw_end = false;
	bool do_highlight_menu_item = false;
	int selected_menu_index = 0;
	float menu_timer = 0.0f;
	void DrawWindow(int x, int y, int w, int h);
	void DrawWindowEndSection();


};

