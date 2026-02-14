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


	std::string SceneLink = "HELLO_SCENE_FILE.txt";
	std::string ScenePath = "Assets/Scenes/";
	vec3d CameraStart;
	vec3d CameraEnd;
	float FadeIn = 255;

	void Tick(float DeltaTime);
	bool battleFinished = false;
};

