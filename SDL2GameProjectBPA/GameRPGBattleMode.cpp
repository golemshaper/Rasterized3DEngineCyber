#include "GameRPGBattleMode.h"
#include "TextSprites.h"


void GameRPGBattleMode::Initialize()
{
	//WE DON"T INIT HERE.
	//The game GameRPGWorld controls us instead of the main.
}

void GameRPGBattleMode::Setup(DrawScratchSpace* scratchspace, TextFileReader* txt, TextSprites* txt_sprites)
{
	MyScratch = scratchspace;
	TextFileReaderRef = txt;
	MyTextSprites = txt_sprites;
	battleFinished = false;
}

void GameRPGBattleMode::StartBattle()
{
	totalTime = 0.0f;
}

void GameRPGBattleMode::LoadScene()
{
	//Load battle mode scene here!
	//The scene should change based on terrain.
	//I may also choose to have two scenes, one for the background, and one for the battle systems, like spawn points and cameras.
	//I may also want to just load the background model mesh as its own file, and not make it part of the battle scene, since it will just contain a single BKG data.
	//Consider making an Exclude tag that makes a model either not export or not import...

	//choose a maximum number of fighters in the battle, and treat all the same, with a component struct for stats.
	//make an encounters text file battles are loaded from!
	//Or get lazy and embed them in scene data somehow...
	//but probably, just make an INI file like format for that sort of game data like in YIIK
}

void GameRPGBattleMode::Tick(float DeltaTime)
{
	totalTime += DeltaTime;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->DrawTextAtPos(3, 3, RGB_Yellow, "Battle mode is running", MyTextSprites);
	if (totalTime >= 1.0f)
	{
		battleFinished = true;
	}

}