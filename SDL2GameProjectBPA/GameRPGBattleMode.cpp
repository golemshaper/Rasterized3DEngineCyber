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
	FadeIn = 255.0f;
	totalTime = 0.0f;
	LoadScene(SceneLink);
}

void GameRPGBattleMode::LoadScene(std::string SceneFileName)
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


	//When you kill an enemy, you will have a chance of absorbing their essense and turning in to that enemy if you are the elf.
	//I will make a function that takes a mesh, and a target mesh, and shrink wraps to the target mesh, so that I can make a transformation sequence using the silhoette as a mask. Sailormoon transform shit.

	SceneParserObject = SceneParser->ParseSceneFromFile(ScenePath + SceneFileName, "Assets/Models/");
	totalTime = 0.0f;
	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		if (SceneParserObject.scene_objects[i].HasTagStringCompare("Hidden"))
		{
			SceneParserObject.scene_objects[i].visible = false;
		}
		if (SceneParserObject.scene_objects[i].HasTagStringCompare("CamLoc"))
		{
			CameraStart = SceneParserObject.scene_objects[i].pos;
		}
		if (SceneParserObject.scene_objects[i].HasTagStringCompare("CameraAim"))
		{
			CameraEnd = SceneParserObject.scene_objects[i].pos;
		}
	}
}

void GameRPGBattleMode::Tick(float DeltaTime)
{
	//BEGIN FRAME
	totalTime += DeltaTime;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	
	//EXIT BATTLE TEMP TIMER
	if (totalTime >= 1.0f)
	{
		battleFinished = true;
	}
	MyScratch->ZWriteOn = true;
	MyScratch->SetCamera(CameraStart, CameraEnd);
	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		//MODEL
		if (SceneParserObject.scene_objects[i].visible == false)
		{
			continue; 
		}
		int MeshId = SceneParserObject.scene_objects[i].model_id;
		int TextureID = SceneParserObject.scene_objects[i].texture_id;
		//TEXTURE
		if (TextureID != -1 && SceneParserObject.TexturePacks.size() >= TextureID)
		{
			MyScratch->TextureDrawOn = true;
			MyScratch->SetTexture(
				SceneParserObject.TexturePacks[TextureID].TextureData,
				SceneParserObject.TexturePacks[TextureID].width,
				SceneParserObject.TexturePacks[TextureID].height
			);
		}
		else
		{
			MyScratch->TextureDrawOn = false;
		}
		//DRAW
		MyScratch->DrawMesh(
			SceneParserObject.Meshes[MeshId],
			SceneParserObject.scene_objects[i].pos,
			SceneParserObject.scene_objects[i].rot,
			SceneParserObject.scene_objects[i].scale,
			false
		);
	}
	if (FadeIn > 0.0f)
	{
		FadeIn -= 125.0f * DeltaTime;
		MyScratch->DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, RGB{ 0,0,0,(int)FadeIn});
	}
	MyScratch->DrawTextAtPos(3, 3, RGB_Yellow, "Battle mode is running", MyTextSprites);

}