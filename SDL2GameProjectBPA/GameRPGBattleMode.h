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
	void LoadScene();
	SceneFileParser* SceneParser;
	Scene CurrentScene;

	void Tick(float DeltaTime);
	bool battleFinished = false;
};

