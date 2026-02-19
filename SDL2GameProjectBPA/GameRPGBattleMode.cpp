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

	PlayerParty[0].name = "Hero";
	
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

	SceneParserObject = SceneParser->ParseSceneFromFile(ScenePath + SceneFileName, "Assets/Models/","Assets/");
	totalTime = 0.0f;
	curPartySP = 0;
	curEnemySP = 0;

	Tag_Hidden = SceneParserObject.GetTagID("Hidden");
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
		if (SceneParserObject.scene_objects[i].HasTagStringCompare("PlayerSpawn"))
		{
			PartySpawnPoints[curPartySP] = i;

			curPartySP++;
		}
		if (SceneParserObject.scene_objects[i].HasTagStringCompare("EnemySpawn"))
		{
			EnemySpawnPoints[curEnemySP] = i;
			curEnemySP++;
		}
	}
	// set party member to spawn point location
	for (int i = 0; i < PartySize; i++)
	{
		if (PlayerParty[i].inParty==false)continue;
		//loc
		PlayerParty[i].loc = SceneParserObject.scene_objects[PartySpawnPoints[i]].pos;
		
		//rot
		vec3d dir = MyScratch->LookAtRotation(PlayerParty[i].loc, vec3d{ 0,0,0 });
		float yaw = atan2f(-dir.x, -dir.z);
		PlayerParty[i].rot.y = yaw;
	}
}

void GameRPGBattleMode::Tick(float DeltaTime)
{
	//BEGIN FRAME
	totalTime += DeltaTime;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->PushBackDepthBuffer(100);
	//EXIT BATTLE TEMP TIMER
	if (totalTime >= 2.0f)
	{
		battleFinished = true;
	}
	MyScratch->ZWriteOn = true;
	MyScratch->SetCamera(CameraStart, CameraEnd);
	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		MyScratch->MaxLambertDarkness = abs(sin(i+totalTime) * 0.5f);
		//MODEL
		if (SceneParserObject.scene_objects[i].visible == false)
		{
			continue; //Hidden
		}
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Hidden))
		{
			continue; //Hidden
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

	//Common texture set
	MyScratch->TextureDrawOn = true;
	MyScratch->SetTexture(Palette, w64, h64);

	//DRAW BATTLE AGENTS
	//HERO
	for (int i = 0; i < PartySize; i++)
	{
		MyScratch->MaxLambertDarkness = abs(sin((SceneParserObject.scene_objects.size()+i)+totalTime)*0.5f);
		if (PlayerParty[i].inParty == false)
		{
			continue;
		}
		//shadow (CONSIDER DOING A SHADOW PASS ON ALL AGENTS AND FADING IT LATER)
		MyScratch->MeshColor = { 1,1,1,255 };
		MyScratch->TextureDrawOn = false;
		MyScratch->DrawUnlit = true;
		MyScratch->UseGouraudShading = false;
		MyScratch->PushBackDepthBuffer(100);
		PlayerParty[i].scale = vec3d{
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)),
			0.1f,
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)),
		};
		//draw shadow after fade to avoid visual bug until I fix it
		if(FadeIn<=0.0f)MyScratch->DrawMesh(PlayerParty[i].Idle, PlayerParty[i].loc, PlayerParty[i].rot, PlayerParty[i].scale);
		MyScratch->PushBackDepthBuffer(100);
		MyScratch->MeshColor = RGB_White;
		MyScratch->DrawUnlit = false;
		MyScratch->UseGouraudShading = true;
		MyScratch->TextureDrawOn = true;
		//Squash and stretch
		PlayerParty[i].scale = vec3d{
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)), 
			PartyMemberScale + (abs(cos(i + totalTime * 12.0f) * 0.1f)),
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)),
		};
		MyScratch->DrawMesh(PlayerParty[i].Idle, PlayerParty[i].loc, PlayerParty[i].rot, PlayerParty[i].scale);
	}


	//ENEMY (Merge with hero?)
	for (int i = 0; i < PartySize; i++)
	{
		MyScratch->MaxLambertDarkness = abs(sin((SceneParserObject.scene_objects.size() + i) + totalTime) * 0.5f);
		if (EnemyParty[i].inParty == false)
		{
			continue;
		}
		//Squash and stretch
		EnemyParty[i].scale = vec3d{
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)),
			PartyMemberScale + (abs(cos(i + totalTime * 12.0f) * 0.1f)),
			PartyMemberScale + (abs(sin(i + totalTime * 12.0f) * 0.1f)),
		};
		MyScratch->DrawMesh(EnemyParty[i].Idle, EnemyParty[i].loc, EnemyParty[i].rot, EnemyParty[i].scale);
	}
	MyScratch->ClearZBufffer();
	//FADE FX
	if (FadeIn > 0.0f)
	{
		FadeIn -= 125.0f * DeltaTime;
		int FadeAsInt = (int)FadeIn;
		
		/*if (FadeAsInt > 1)FadeIn = 1;
		if (FadeAsInt < 0)FadeIn = 0;*/
		MyScratch->ZWriteOn = false;
		MyScratch->DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, RGB{ 1,1,1,abs(FadeAsInt)});
	}
	MyScratch->DrawTextAtPos(3, 3, RGB_Yellow, "Battle mode is running", MyTextSprites);

}