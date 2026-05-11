#include "GameRPGBattleMode.h"
#include "TextSprites.h"
#include "TextFileReader.h"

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
	Reader = new TextFileReader();
	Reader->ReadText();
}

void GameRPGBattleMode::StartBattle()
{
	FadeIn = 255.0f;
	totalTime = 0.0f;
	SetEncounterDummyData();
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
		PlayerParty[i].home_loc = SceneParserObject.scene_objects[PartySpawnPoints[i]].pos;
		PlayerParty[i].target_loc = SceneParserObject.scene_objects[EnemySpawnPoints[0]].pos; //example only!
		
		//rot
		vec3d dir = MyScratch->LookAtRotation(PlayerParty[i].loc, vec3d{ 0,0,0 });
		float yaw = atan2f(-dir.x, -dir.z);
		PlayerParty[i].rot.y = yaw;
	}
	//load enemy encounter party
	for (int i = 0; i < PartySize; i++)
	{
		if (EnemyParty[i].inParty == false)continue;
		//loc
		EnemyParty[i].loc = SceneParserObject.scene_objects[EnemySpawnPoints[i]].pos;
		EnemyParty[i].home_loc = SceneParserObject.scene_objects[EnemySpawnPoints[i]].pos;
		EnemyParty[i].target_loc = SceneParserObject.scene_objects[EnemySpawnPoints[0]].pos; //example only!

		//rot
		vec3d dir = MyScratch->LookAtRotation(EnemyParty[i].loc, vec3d{ 0,0,0 });
		float yaw = atan2f(dir.x, dir.z);
		EnemyParty[i].rot.y = yaw;
	}
}

void GameRPGBattleMode::Tick(float DeltaTime)
{
	//BEGIN FRAME
	MyScratch->UsePillowShadeNormals = false;
	MyScratch->PillowShadeAmount = 0.85f;

	MyScratch->SetCameraFOV(90+ (sin(totalTime*2)*10)); //CAMERA FOV PULSE TEST
	MyScratch->MaxLambertDarkness = 0.0f;
	totalTime += DeltaTime;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->PushBackDepthBuffer(100);
	//EXIT BATTLE TEMP TIMER
	
	if (totalTime >= 2.0f)
	{
		//TEST ONLY
		
		//setting battleFinished to true will end the battle mode screen
		//	battleFinished = true;
	}
	MyScratch->ZWriteOn = true;
	MyScratch->SetCamera(CameraStart, CameraEnd);
	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		//////MyScratch->MaxLambertDarkness = abs(sin(i+totalTime) * 0.5f);
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


	//MOVE AGENTS
	
	MoveBattleAgentToTarget(0,PlayerParty,1,EnemyParty,MyScratch->Modulous(totalTime));

	//DRAW BATTLE AGENTS
	MyScratch->UsePillowShadeNormals = true;
	//HERO
	for (int i = 0; i < PartySize; i++)
	{
		//MyScratch->MaxLambertDarkness = abs(sin((SceneParserObject.scene_objects.size()+i)+totalTime)*0.5f);
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
		vec3d shadowLoc = PlayerParty[i].loc;
		shadowLoc.y = PlayerParty[i].home_loc.y;
		if(FadeIn<=0.0f)MyScratch->DrawMesh(PlayerParty[i].Idle, shadowLoc, PlayerParty[i].rot, PlayerParty[i].scale);
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
		DFShdow();

		//MyScratch->CopyBufferToBuffer( MyScratch->LastAlphaCopy,MyScratch->MainSpace); //DEBUG LAST ALPHA DRAW
		
		
		
		
		//TODO: Make a function that takes the alpha buffer, and shifts it over in space,
		// and then multiplies the result by a color on top of the model, masked out by the original alpha location!
	}


	//ENEMY (Merge with hero?)
	for (int i = 0; i < PartySize; i++)
	{
		//MyScratch->MaxLambertDarkness = abs(sin((SceneParserObject.scene_objects.size() + i) + totalTime) * 0.5f);
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
		DFShdow();
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
	//Load text: RPGBattleHypeTxt
	RequestedText = "RPGBattleHypeTxt";
	MyScratch->DrawTextAtPos(3, 3, RGB_Yellow, Reader->GetStringFromSheetTag(RequestedText.c_str()), MyTextSprites,1.0f);

	//UI
	DrawBattleMenu();
	menu_timer += DeltaTime;
	if (menu_timer >= 0.25f)
	{
		menu_timer = 0.0f;
		selected_menu_index++;
		selected_menu_index = selected_menu_index % 5;
	}

	//RELOAD
	MyScratch->Input->Tick(DeltaTime);
	if (MyScratch->Input->GetToggleDepthKey())
	{
		MyScratch->Input->ResetToggleDepthKey();
		LoadScene(SceneLink);
		return;
	}
	
}


void GameRPGBattleMode::DFShdow()
{
	MyScratch->CopyBufferToBuffer(MyScratch->LastAlphaCopy, OriginalMeshAlpha);
	MyScratch->CopyBufferToBuffer(MyScratch->MainSpace, OriginalPixels);
	////Offset alpha is in the extra buffer
	MyScratch->ClearBuffer(MyScratch->ExtraBuffer);
	MyScratch->ShiftBufferXY(MyScratch->LastAlphaCopy, MyScratch->ExtraBuffer, SCREEN_X, SCREEN_Y, -3, 3);
	////Original alpha is in the main space
	MyScratch->CopyBufferToBuffer(OriginalMeshAlpha, MyScratch->MainSpace);
	MyScratch->ApplyMaskTypeTwo();
	
	MyScratch->CopyBufferToBuffer(MyScratch->ExtraBuffer, MyScratch->MainSpace);
	MyScratch->ColorizeBuffer(MyScratch->MainSpace, RGB{ 400,400,400,255 });
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->CopyBufferToBuffer(OriginalPixels, MyScratch->MainSpace);
	for (int i = 0; i < TOTAL_PIXELS; ++i)
	{
		if (MyScratch->ExtraBuffer[i].r <= 12) { continue; }

		MyScratch->MainSpace[i] = ( MyScratch->MainSpace[i]*(MyScratch->ExtraBuffer[i]))/1024;
	}
}



void GameRPGBattleMode::MoveBattleAgentToTarget(int MoveAgentID, BattleAgent(&MoveAgentPool)[PartySize], int AgentTargetID, BattleAgent(&TargetAgentPool)[PartySize], float amount)
{
	vec3d a = MoveAgentPool[MoveAgentID].home_loc;
	vec3d b = TargetAgentPool[AgentTargetID].loc;
	MoveAgentPool[MoveAgentID].target_loc = b;
	MoveAgentPool[MoveAgentID].loc = MyScratch->Arc(a,b,2.0f,amount);
}

void GameRPGBattleMode::SetEncounterDummyData()
{
	//For testing/experimenting on how encounters will be set
	
	//EnemyParty[0].Idle = PlayerParty[0].Idle; //copy player for now...
	//EnemyParty[0].Attack = PlayerParty[0].Attack; //copy player for now...
	//EnemyParty[0].inParty = PlayerParty[0].inParty; //copy player for now...
	//EnemyParty[0].alive = PlayerParty[0].alive; //copy player for now...
	for (int i = 0; i < PartySize; i++)
	{
		EnemyParty[i].Idle = PlayerParty[0].Idle; //copy player for now...
		EnemyParty[i].Attack = PlayerParty[0].Attack; //copy player for now...
		EnemyParty[i].inParty = PlayerParty[0].inParty; //copy player for now...
		EnemyParty[i].alive = PlayerParty[0].alive; //copy player for now...
	}
}

void GameRPGBattleMode::DrawBattleMenu()
{
	int startX = 16;
	int startY = 16;
	int width = 55;
	int height = 16;
	
	//For each menu item, draw a new window box, and populate the center with text label
	int num = 5;
	const char* Names[] = {
	"Attack",
	"Skill",
	"Item",
	"Fuse", //fuse self to enemy, or fuse two enemies together to make that enemy stronger.
	"Guard"
	};
	for (int i = 0; i < num; i++)
	{
		if (i == 0) { DrawWindowStartSection(); }
		if (i == num-1) { DrawWindowEndSection(); }
		if (i == selected_menu_index) { DrawWindowHighlight(); }//Highlight selected

		DrawWindow(startX, startY + (i * height), width, height);
		MyScratch->DrawTextAtPos(startX + 5, startY + 5 + (height * i), RGB_NearBlack, Names[i], MyTextSprites);
		RGB HighlightColor = RGB_Grey;
		if (do_highlight_menu_item)
		{
			HighlightColor = RGB_White;
		}
		MyScratch->DrawTextAtPos(startX+4, startY + 4 + (height*i), HighlightColor, Names[i], MyTextSprites);
		do_highlight_menu_item = false;
	}
}

void GameRPGBattleMode::DrawWindowStartSection()
{
	window_draw_start = true;
}

void GameRPGBattleMode::DrawWindowHighlight()
{
	do_highlight_menu_item = true;
}

void GameRPGBattleMode::DrawWindow(int x, int y, int w, int h)
{
	RGB u = RGB{ 0,0,245,128 };
	RGB d = RGB{ 0,222,0,128 };
	RGB l = RGB{ 0,0,245,0 };
	RGB r = RGB{ 0,0,111,0 };
	if (!do_highlight_menu_item)
	{
		u = u * 0.5f;
		d = d * 0.5f;
		l = l * 0.5f;
		r = r * 0.5f;
	}
	MyScratch->DrawRectangle(
		x, y, w, h, 
		u,d,l,r
	);
	RGB TopColor = RGB_White;
	RGB BottomColor = RGB_Grey;
	if (!window_draw_start)
	{
		TopColor = { 0,0,0,0 };
	}
	if (!window_draw_end)
	{
		BottomColor = { 0,0,0,0 };
	}
	MyScratch->DrawRectangleOutline(x, y, w, h, TopColor, BottomColor, RGB_White, RGB_Grey);
	MyScratch->DrawRectangleOutline(x+2, y+2, w-4, h-4, TopColor /2, BottomColor /2, RGB_White/2, RGB_Grey/2);
	window_draw_start = false;
	window_draw_end = false;
	
}

void GameRPGBattleMode::DrawWindowEndSection()
{
	window_draw_end = true;
}
