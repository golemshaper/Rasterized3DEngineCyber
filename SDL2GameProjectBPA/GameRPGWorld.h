#pragma once
#include "Game.h"
#include "miniaudio.h"
#include "SceneFileParser.h"
#include "StateMachine.h"
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
	void DrawSceneObjects(float DeltaTime);
	void DrawSingleSceneObject(int objId);
	void DrawSingleSceneObject(int objId, vec3d CustomOffsetPos, Mesh CustomMesh, bool UseCustomMesh);
	void CollisionProcess(int objId);
	void MusicAndFadeIn(float DeltaTime);
	void RenderMovie();
	void DrawBasics(float DeltaTime);


	//Scene
	std::string SceneLink = "RPG_WorldMap.txt";
	std::string ScenePath = "Assets/Scenes/";
	std::string ModelsPath = "Assets/Models/";

	//REMEMBER TO CLEAR THESE INSIDE OF THE LOAD SCENE OBJECT:
	std::vector<int> MeshPropIDs;
	std::vector<int> CharacterNPCIDs;

	SceneFileParser* SceneParser;
	Scene CurrentScene;
	//Scene Tags
	int Tag_Town = -1;
	int Tag_Hidden = -1;
	int Tag_Unlit = -1;
	int Tag_Character = -1;
	int Tag_Animation = -1;
	int Tag_SnapToGround = -1;
	int Tag_RandomWalk = -1;
	//ObjectIDs
	int LightStartID;
	int LightEndID;


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
	RGB* grass;
	RGB* overworldTexture;
	RGB* water;
	RGB* Palette;

	const char* RequestedText = "RpgIntro";
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text = "";
	void TextUpdateTick(float DeltaTime);
	void TextBoxDraw(const char* input);

	int lightning_phase = 0;
	float lightning = 0.0f;
	void LightningFX(int phase, float progress);

	//Animatables
	struct AnimationComp {
		int scene_obj_id;
		Mesh Idle;
		Mesh Walk;
		bool enfOfList = true; //If true, we abort all further animation updating
	};
	static constexpr int MaxComponentCount = 16;
	AnimationComp AnimationComponents[MaxComponentCount];
	int CurrentAnimationComponentIndex = 0;
	void DeleteAnimations();
	void CreateAnimationComp(int OnSceneObjectID);

	//Walkables
	struct RandomWalkComp {
		int scene_obj_id= -1;
		vec3d direction = { 0.0f,0.0f,0.0f };
		float speed = 12.0f;
		float walkForTimeMin = 0.25f;
		float walkForTimeMax = 2.0f;
		float curWalkForTimer = 0.0f;
		float waitTimer = 0.0f;
		bool followRotation = true;
	};
	int CurrentWalkomponentIndex = 0;
	void DeleteRandomWalkComps();
	void CreateRandomWalkComp(int OnSceneObjectID);
	void ProcessWalkComps(float DeltaTime);
	RandomWalkComp RandomWalkComponents[MaxComponentCount];



	
	void ProcessAnimations(float DeltaTime);
	

	//Game State

	StateMachine* sm;
	enum State {
		TitleScreen = 0,
		WorldMap = 1,
		BattleTransition = 2,
		Battle = 3,
		GameOver = 4,
	};

	void SetupStateMachine();
	void StateOverworldUpdate();
	void StateBattleTransitionStart();
	void StateBattleTransitionUpdate();
	void StateBattleUpdate();






	~GameRPGWorld();
};

