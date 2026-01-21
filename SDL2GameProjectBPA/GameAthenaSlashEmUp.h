#pragma once
#include "Game.h"
#include "TextFileReader.h"
#include "StateMachine.h"

class GameAthenaSlashEmUp : public Game
{
	~GameAthenaSlashEmUp();
public:

	StateMachine sm;

	float GameSpeed = 1.0f;
	//Current game mode
	int mode = 0;
	int drawBuffer = 0;
	bool DEBUG_DrawColliders = true;

	float demoWait = 4.0f;
	//primitive
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text = "";

	float angle = 0.0f;

	//pools
		//athena body
	int bullet_count = 64;
	vec3d bullets[64];
		//arc shots
	int bullet_arcshots_count = 44;
	vec3d bullet_arcshots[44];
	vec3d bullet_arcshots_end_target[44];
	float progress_for_arc_shots=0.0f;

	//actors
	vec3d camLocation = { 0,0,0 };
	vec3d psudoCamLocation= { 0,0,0 };
	vec3d player_position = { 0,0,0 };
	vec3d player_move = { 0,0,0 };
	vec3d last_safe_look = { 0,0,0 };
	float player_speed = 4.0f;

	//re-usable
	vec3d working_vector = { 0,0,0 };
	RGB working_color = { 0,0,0,0 };
	float working_float = 0.0f;
	float lightning = 0.0f;
	int lightning_phase = 0;
	int fake_exp_until_stats_container_added = 0;

	//Objects
	TextFileReader* Reader;







	void Initialize();
	void Tick(float DeltaTime);
	void TitleScreenTick(float DeltaTime);
	void GameModeTick(float DeltaTime);
	void DrawWorldOneBKG(float DeltaTime,float mouseX, float mouseY);

	void ReadNewText(const char* input); //call new text
	const char* RequestedText = "Intro";
	void TextBoxDraw(const char* input);
	void LightningFX(int phase, float progress);
	void NewWaveFXDraw(float DeltaTime);
	void MovementUpdate(float DeltaTime);

	void TickArcShots(vec3d start, vec3d end, float DeltaTime);

	void CircleTransition(float reveal);

	void DrawReticle(int x, int y, int radius, float progress);
	void DrawReticle(vec3d pos, int radius, float progress);


	//Actor struct added. Make a function called "Create Actor"
	//Pre create actors on initialize via that Create Actor function, and assign the resulting Actor to a list of actors
	//Iterrate over all actors, rendering them, and adding to their time counter.
	//If an actor is of a certain type, say an enemy, you can add its ID to a list of IDs of that enemy type, and use the ID to index
	//the Actors list, and perform movement based of functions designed to move enemies of that type.
	//That's how I think this'll all work.

	//An Actor is not part of the "Engine" as the requirements will differ between games

	//If you need special drawing functions for multiple actors to work as one, it'll be as simple as keeping all parts in their own list, and using the ID to refer to them.
	//Store an ID for the main character in init.
	//
	//Things like speed and time counter should maybe be in a speeds list or a time counter list, that shares the same index as the actor....... like a crude component system...
	//----------------
	//gameplay structs
	//----------------
	struct Actor {
		Mesh  m{};
		vec3d loc{ 0,0,0 };
		vec3d rot{ 0,0,0 };
		vec3d scale{ 1,1,1 };
		RGB meshColor = { 255,255,255,255 };
		float speed{ 0.0f };
		float timeCounter{ 0.0f };
		vec3d offset_loc{ 0,0,0 };
		vec3d offset_rot{ 0,0,0 };
		bool isAlive = false;
	};
	
	struct Stats {
		int lvl = 1;
		int hp = 10;
		int mp = 5;
		int atk = 1;
		int def = 1;
		int exp = 0;
	};
	//UNDER CONSIDERATION: DO NOT USE UNTIL THEN
	struct LocRotScal
	{
		vec3d loc{ 0,0,0 };
		vec3d rot{ 0,0,0 };
		vec3d scale{ 1,1,1 };
	};
	struct Enemy {
		Stats stats = {};
		LocRotScal transform = {}; //for lack of a better name 
	};
	//I can either make actors the generic "thing" and make "component" like systems for enemies, or I can make one uber actor with stats and transforms...
	//DO WHAT IS FUN TO WORK ON!
	// DECIDE WHAT THAT IS! 
	// THIS IS NOT A 30+ HOUR GAME!!
	//--------------------------------------------------------------------------------------------------------------
	std::vector<Actor> AllActors;
	Stats PlayerStats;
	int PlayerID = -1;
	std::vector<Mesh> PlayerMeshSequence;
	int MonkeyID = -1;
	int TeaPotID = -1;
	float temp_player_flip = 1.0f; //temp until animation system added
	bool flip_limit_once = false; //temp until animation system added
	int CreateActor(const Actor& a);
	void DrawActorsFromList(const std::vector<Actor>& actors, bool DrawDifferNow = false);

	bool IsColliding(vec3d a, vec3d b, float radius);
	bool IsColliding2D(vec3d a, vec3d b, float radius);
	//you could collide automatically with every actor if you stored their previous safe location prior to moving them, and then loop through, and reset any actors touching



	//MESHES:
	Mesh CurrentAthenaFrame;
	Mesh AlienMesh;


	//----------------
	//Game States
	//----------------
	const int FirstStateId = 0;
	const int State_AthenaIdle = 1;
	const int State_AthenaAttack = 2;

	const int NoStateID = -1;
	void StateMachineHelloWorldTick();
	void PlayerIdleState();
	void PlayerAttackState();



	//----------------
	//HUD
	//----------------
	void DrawStatsAtLocation(Stats& stats, vec3d loc, bool useFull);
	void DrawHUD(float DeltaTime);


	//This is an array that is cleared and modified every tick.
	//the max number of enemies on screen should be the size of this array
	vec3d TargetableLocations[30];
	int currentTargetable = 0;
	void ClearTargetableLocAry(vec3d* TargetableList, int size_of_array);
	void AddTargetableLOC(vec3d* TargetableList, int size_of_array, vec3d target);
	void AddTargetableLOC(vec3d target);
	vec3d GetClosestVectorFromList(vec3d TestAgainst, vec3d* TargetableList, int size_of_array);

	//----------------
	//FX
	//----------------
	RGB BlurBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
	void AccumulatedBlur(float strength);
};

