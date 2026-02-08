#include "GameRPGWorld.h"
#include "MonkeyMesh.h"
#include <string>

#include "TextSprites.h"
#include "TextFileReader.h";

#include "ModelFileParser.h"
#include "BMPWriter.hpp"
#include "BMPReader.hpp"
#include "ThirdPersonMovement.h"
#include "miniaudio.h"


void GameRPGWorld::Initialize()
{
	MyScratch = new DrawScratchSpace();
	MyScratch->Initialize();
	MyScratch->UseGouraudShading = true;
	MyScratch->UseFogHackyShading = false;
	MyScratch->UseDepthFog = true;

	SceneParser = new SceneFileParser();
	PlayerMovement = new ThirdPersonMovement();
	PlayerMovement->Pos = vec3d{ 0,-1.3f,0 };
	ModelFileParser parser;
	MyTextSprites = new TextSprites();
	Reader = new TextFileReader();
	Reader->ReadText();
	//LoadedMesh = parser.ParseFromStr(text);
	//MY PC: C:\Users\brian\source\repos\Rasterized3DEngine\SDL2GameProjectBPA\Assets
	LoadedMesh = parser.ParseModelFromFile("Assets/olexa.txt");
	LoadedMesh2 = parser.ParseModelFromFile("Assets/CubeTiledUvs.txt");

	//player
	PlayerMesh_Idle = parser.ParseModelFromFile("Assets/RPGWarriorRohan.txt");
	PlayerMesh_Walk = parser.ParseModelFromFile("Assets/RPGWarriorRohan_Walk_F1.txt");
	PlayerMesh = PlayerMesh_Idle;


	TerrrainMesh = parser.ParseModelFromFile("Assets/OverworldTerrain.txt"); 
	WaterPlaneMesh = parser.ParseModelFromFile("Assets/WaterPlaneCutoutCenter.txt");
	//LoadedMesh2 = parser.ParseFromFile("Assets/cube_model.txt");

	//How to animate:
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F0.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F1.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F1.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F2.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F3.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F4.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F5.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F6.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F0.txt"));
	MeshSequence.push_back(parser.ParseModelFromFile("Assets/Athena_F0.txt"));


	int wGB = 128, hGB = 112;
	Image01 = ReadBMP("Assets/001.bmp", wGB, hGB);


	int w2 = 16; int h2 = 16;
	grass = ReadBMP("Assets/grass.bmp", w2, h2);


	int w256 = 256; int h256 = 256;
	overworldTexture = ReadBMP("Assets/Overworld.bmp", w256, h256);


	int w32 = 32; int h32 = 32;
	water = ReadBMP("Assets/water.bmp", w32, w32);


	int w3 = 16; int h3 = 16;
	Image03 = ReadBMP("Assets/red_brick.bmp", w3, h3);
	MyScratch->SetTexture(Image03,w3,h3);
	
	int w64 = 64; int h64 = 64;
	Palette = ReadBMP("Assets/BasicPalette.bmp", w64, h64);

	ma_result r = ma_engine_init(NULL, &audioEngine);
	printf("engine init: %d\n", r);
	ma_engine_play_sound(&audioEngine, "Assets/noise_transition.wav", NULL);


	//TODO Call LoadSceneFiles()
	LoadSceneFiles();
	
}
void GameRPGWorld::LoadSceneFiles()
{
	LoadSceneFiles(SceneLink);
}
void GameRPGWorld::LoadSceneFiles(std::string SceneFileName)
{
	CurrentScene = SceneParser->ParseSceneFromFile(ScenePath + SceneFileName, "Assets/Models/","Assets/");
	totalTime = 0.0f;
	
	MeshPropIDs.clear(); //Collect non-terrain meshes here
	for (int i = 0; i < CurrentScene.scene_objects.size(); i++)
	{
		if (CurrentScene.scene_objects[i].HasTagStringCompare("Terrain"))
		{
			int MeshId = CurrentScene.scene_objects[i].model_id;
			TerrrainMesh = CurrentScene.Meshes[MeshId];
		}
		else if (CurrentScene.scene_objects[i].HasTagStringCompare("TerrainCollider"))
		{
			int MeshId = CurrentScene.scene_objects[i].model_id;
			TerrrainMeshCollider = CurrentScene.Meshes[MeshId];
		}
		else if (CurrentScene.scene_objects[i].HasTagStringCompare("PlayerSpawn"))
		{
			if (loadPositionOnce) //don't merge with the if-else above, that breaks the logic
			{
				PlayerMovement->Pos = CurrentScene.scene_objects[i].pos;
				vec3d CameraLocation = PlayerMovement->Pos;
				float CamOffsetY = 5.0f;
				vec3d CamRotation = vec3d{ MyScratch->Input->GetMovementX(), CamOffsetY, 17.5f };
				CameraSmoothRotation = CamRotation;
				CameraSmoothLocation = CameraLocation;
				loadPositionOnce = false;
			}
		}
		else
		{
			//WARNING THIS IS THE END OF AN IF-ELSE CHAIN. THE ABOVE OBJECTS DID NOT GET PLACED IN THE DRAWING LIST!
			MeshPropIDs.push_back(i);
		}
	}
}

void GameRPGWorld::MusicAndFadeIn(float DeltaTime)
{
	if (!MusicLimitOnce && fStartMusicTimer <= 0.0f)
	{
		MusicLimitOnce = true;
		//ma_sound_init_from_file(&audioEngine, "Assets/e-thena.wav", 0, NULL, NULL, &music);
		ma_sound_init_from_file(&audioEngine, "Assets/e-thena.wav",MA_SOUND_FLAG_DECODE, NULL, NULL, &music);
		ma_sound_set_looping(&music, MA_TRUE);
		ma_sound_start(&music);
		EnableTextbox = true;
	}
	else if (MusicLimitOnce==false)
	{
		fStartMusicTimer -= DeltaTime;
		float fade = (fStartMusicTimer) / 1.5f;
		MyScratch->DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, RGB{ 0,0,0,(int)(255 * fade) });
	}
}
void GameRPGWorld::Tick(float DeltaTime)
{
	
	//Instead of ticking directly, we will have a state machine for the game modes. The scene file itself will tell the engine what mode the scene should be in.
	//A battle scene file, will change to the battle mode state, and when the battle scene load, the game will be in battle mode!
	
	//TODO Implement Gouraud shading.

	if (MyScratch->Input->GetToggleDepthKey())
	{
		MyScratch->Input->ResetToggleDepthKey();
		LoadSceneFiles();
		return;
	}
	//Music
	//DeltaTime = 1.0f / 30.0f; //classic slowdown
	//---------------
	//Setup:
	//---------------
	totalTime += DeltaTime;
	animTimer += DeltaTime;
	//MyScratch->LightDir = { sin(totalTime),cos(totalTime),sin(totalTime) };
	
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->TextureDrawOn = false;
	int w16 = 16; int h16 = 16;
	int w32 = 32; int h32 = 32;
	int wGB = 128; int hGB = 112;
	int w256 = 256; int h256 = 256;
	int w64 = 64; int h64 = 64;
	vec3d PlayerScale = vec3d{ 2.5f,2.5f,2.5f };


	MyScratch->MeshColor = RGB_White;

	//---------------
	//PLAYER MOVEMENT:
	//---------------
	PlayerMovement->ApplyMovement(DeltaTime, MyScratch);
	PlayerMovement->ApplyGroundSnap(TerrrainMeshCollider, MyScratch, vec3d{ 0,-1.3f,0 });
	//animate
	vec3d PlayerOffset = vec3d{ 0,abs(sin(totalTime * 12.0f)) * -0.2f,0.0f };
	if (PlayerMovement->IsMoving())
	{
		PlayerMesh = MyScratch->MorphMesh(PlayerMesh_Idle, PlayerMesh_Walk, sin(totalTime * 12.0f) * 0.5f);
	}
	else
	{
		PlayerMesh = PlayerMesh_Idle;
		PlayerOffset = vec3d{ 0,0,0 };
	}

	vec3d PlayerLocation = PlayerMovement->Pos; //CIRCLE: vec3d{ sin(totalTime) * 10.0f,-12.5f,cos(totalTime) * 8.0f };
	//---------------
	//CAMERA:
	//---------------
	//Camera code should move in to the ThridPersonMovement function once I get a chance!
	PlayerMovement->CameraOrientation.x += PlayerMovement->CameraRotationSpeed * DeltaTime * MyScratch->Input->GetCameraXAxis();
	float orbitAngle = PlayerMovement->CameraOrientation.x;
	float c = cos(orbitAngle);
	float s = sin(orbitAngle);
	vec3d baseOffset = { 0.0f, -5.9f, -17.5f };
	vec3d offset;
	offset.x = baseOffset.x * c + baseOffset.z * s;
	offset.y = baseOffset.y;
	offset.z = -baseOffset.x * s + baseOffset.z * c;
	vec3d CameraLocation = PlayerLocation + offset;


	float CamOffsetY = 5.0f;
	vec3d CamRotation = vec3d{ MyScratch->Input->GetMovementX(), CamOffsetY, 17.5f };

	CamRotation = PlayerLocation - CameraLocation + vec3d{ MyScratch->Input->GetMovementX(),0,0 };


	CameraSmoothRotation = MyScratch->Lerp(CameraSmoothRotation, CamRotation, (PlayerMovement->Speed / 2.0f) * DeltaTime);
	CameraSmoothLocation = MyScratch->Lerp(CameraSmoothLocation, CameraLocation, (PlayerMovement->Speed / 2.0f) * DeltaTime);
	MyScratch->SetCamera_Legacy(CameraSmoothLocation, CameraSmoothRotation);
	//MyScratch->SetCameraFOV(90);
	MyScratch->SetCameraFOV(65);

	//Push zbuffer back to make more "room" for the depth of the scene
	MyScratch->ClearZBufffer();

	//---------------
	// Lighting
	// --------------
	//MyScratch->LightDir = vec3d{ 12,32,64 };
	//---------------
	//collision + offset
	//---------------
	PlayerLocation = MyScratch->SnapToMesh(PlayerLocation, TerrrainMeshCollider, vec3d{ 0,0,0 });
	vec3d PlayerLocationMirrored = { PlayerLocation.x,-PlayerLocation.y + 29.0f, PlayerLocation.z };
	//---------------
	//water:
	//---------------
	MyScratch->ZWriteOn = false; //Depth  off so we can draw as far as possible!
	vec3d WaterLocation = { PlayerLocation.x,0.0f,PlayerLocation.z };
	MyScratch->SetTexture(water, w32, h32);
	MyScratch->TextureDrawOn = true;
	//Scroll the uvs, and add the water plane location to make water look infinite
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * 0.25f) + (WaterLocation.x * 0.05f),(totalTime * 0.25f) - (WaterLocation.z * 0.05f) }; //Scrolling UV effect. Use this for water later!
	//Wave mesh:
	Mesh wave = MyScratch->WaveMesh(WaterPlaneMesh, totalTime * 12.0f, 0.25f);
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//Water second layerFX overlay:
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * -0.25f) + (WaterLocation.x * 0.05f),(totalTime * -0.25f) - (WaterLocation.z * 0.05f) }; //Scrolling UV effect. Use this for water later!
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });

	//Reflection:
	MyScratch->DrawMesh(PlayerMesh, PlayerLocationMirrored, vec3d{ 0,PlayerMovement->GetYaw(),0 }, vec3d{ PlayerScale.x,-PlayerScale.y,PlayerScale.z });
	MyScratch->BlendBuffers(0.25f + abs(sin(totalTime)) * 0.5f); //blend two water layers
	MyScratch->ClearZBufffer();//don't need this

	//---------------
	//terrain:
	//---------------
	MyScratch->PushBackDepthBuffer(20000); //Give us pleanty of space to draw the terrain!
	MyScratch->ZWriteOn = true;
	MyScratch->UvOffsetGlobal = vec2d{ 0.0f,0.0f };
	MyScratch->SetTexture(overworldTexture, w256, h256);
	MyScratch->TextureDrawOn = true;
	MyScratch->DrawMesh(TerrrainMesh, vec3d{ 0,0,0 }, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//AccumulatedBlur(0.75f); //Blur only BKG if called here!
	//---------------
	//player's shadow:
	//---------------
	MyScratch->MeshColor = RGB_Black;
	MyScratch->MeshColor.a = 128;
	MyScratch->TextureDrawOn = false;
	MyScratch->PushBackDepthBuffer(90);
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation + vec3d{ 0,0,0 }, vec3d{ 0,PlayerMovement->GetYaw(),0 }, vec3d{ PlayerScale.x,0.1f,PlayerScale.z });
	MyScratch->BlendBuffers(0.5f);
	//---------------
	//player:
	//---------------
	MyScratch->UseDepthFog = false;
	MyScratch->UseFogHackyShading = true;
	MyScratch->UseGouraudShading = false;
	MyScratch->SetTexture(Palette, w64, h64);
	GI_Lighting = MyScratch->Lerp(GI_Lighting, (MyScratch->SnapToMeshTriColor) * 2.5f, 6.0f * DeltaTime);//psudo lighting
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	MyScratch->TextureDrawOn = true;


	//edge light
	MyScratch->MeshColor = GI_Lighting * 2.0f;
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation + PlayerOffset - vec3d{ 0.09f,0.09f,0.0f }, vec3d{ 0,PlayerMovement->GetYaw(),0 }, PlayerScale);
	MyScratch->PushBackDepthBuffer(32);
	//end edge light
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	//normal render
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation + PlayerOffset, vec3d{ 0,PlayerMovement->GetYaw(),0 }, PlayerScale);

	MyScratch->UseDepthFog = true;
	MyScratch->UseFogHackyShading = false;
	MyScratch->UseGouraudShading = true;


	//TEXT MAPPED TO PLAYER:
	//vec3d player_2d_loc = MyScratch->Get2DPointFromLastLocation();
	//MyScratch->DrawTextDropShadow(player_2d_loc.x, player_2d_loc.y, RGB_White, "Hello", MyTextSprites, 1.0f);

	//---------------
	//props:
	//---------------
	MyScratch->MeshColor = RGB_White;
	MyScratch->SetTexture(Image03, w16, w16);
	vec3d BoxPropLoc = vec3d{ -53.478f,1.48093f,-29.807f };
	MyScratch->DrawMesh(LoadedMesh2, BoxPropLoc, vec3d{ 0,totalTime,0 }, vec3d{ 1.0f,1.0f,1.0f }); //position copied from blender, but swapped y and -z


	////---------------
	////Scene file objects:
	////---------------
	for (int i = 0; i < MeshPropIDs.size(); ++i)
	{
		int objId = MeshPropIDs[i];

		int TextureID = CurrentScene.scene_objects[objId].texture_id;

		//TEXTURE
		if (TextureID != -1 && CurrentScene.TexturePacks.size() >= TextureID)
		{
			MyScratch->TextureDrawOn = true;
			MyScratch->SetTexture(
				CurrentScene.TexturePacks[TextureID].TextureData,
				CurrentScene.TexturePacks[TextureID].width,
				CurrentScene.TexturePacks[TextureID].height
			);
		}
		//MyScratch->SetTexture(overworldTexture, w256, h256);
		//MESH
		MyScratch->DrawMesh(
			CurrentScene.Meshes[CurrentScene.scene_objects[objId].model_id],
			CurrentScene.scene_objects[objId].pos,
			CurrentScene.scene_objects[objId].rot,
			CurrentScene.scene_objects[objId].scale,
			false);
	}
	//---------------
	//FX:
	//---------------
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->BrightnessContrastOnBuffer(MyScratch->MainSpace, 0.7f, 2.5f);
	MusicAndFadeIn(DeltaTime);

	//---------------
	//Text
	//---------------
	TextUpdateTick(DeltaTime);
	if (MyScratch->SquaredDistance2D(PlayerLocation, BoxPropLoc) <= 1.0f) {
		RequestedText = "RpgNpcBox";
	}

	/*if (MyScratch->Input->GetFireOneHold())
	{
	//LOAD A  MODEL IN REAL TIME!
		ModelFileParser parser;
		TerrrainMesh = parser.ParseModelFromFile("Assets/DomeModel.txt");
	}*/
}
void GameRPGWorld::OldTick(float DeltaTime)
{
	
	//Music
	//DeltaTime = 1.0f / 30.0f; //classic slowdown
	//---------------
	//Setup:
	//---------------
	totalTime += DeltaTime;
	animTimer += DeltaTime;
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->TextureDrawOn = false;
	int w16 = 16; int h16 = 16;
	int w32 = 32; int h32 = 32;
	int wGB = 128; int hGB = 112;
	int w256 = 256;int h256 = 256;
	int w64 = 64; int h64 = 64;
	vec3d PlayerScale = vec3d{ 2.5f,2.5f,2.5f };
	

	MyScratch->MeshColor = RGB_White;

	//---------------
	//PLAYER MOVEMENT:
	//---------------
	PlayerMovement->ApplyMovement(DeltaTime, MyScratch);
	PlayerMovement->ApplyGroundSnap(TerrrainMesh, MyScratch, vec3d{ 0,-1.3f,0 });
	//animate
	vec3d PlayerOffset = vec3d{ 0,abs(sin(totalTime * 12.0f)) * -0.2f,0.0f };
	if (PlayerMovement->IsMoving()) 
	{
		PlayerMesh = MyScratch->MorphMesh(PlayerMesh_Idle, PlayerMesh_Walk, sin(totalTime * 12.0f) * 0.5f);
	}
	else
	{
		PlayerMesh = PlayerMesh_Idle;
		PlayerOffset = vec3d{ 0,0,0 };
	}

	vec3d PlayerLocation = PlayerMovement->Pos; //CIRCLE: vec3d{ sin(totalTime) * 10.0f,-12.5f,cos(totalTime) * 8.0f };
	//---------------
	//CAMERA:
	//---------------
	//Camera code should move in to the ThridPersonMovement function once I get a chance!
	PlayerMovement->CameraOrientation.x += PlayerMovement->CameraRotationSpeed * DeltaTime * MyScratch->Input->GetCameraXAxis();
	float orbitAngle = PlayerMovement->CameraOrientation.x;
	float c = cos(orbitAngle);
	float s = sin(orbitAngle);
	vec3d baseOffset = { 0.0f, -5.9f, -17.5f };
	vec3d offset;
	offset.x = baseOffset.x * c + baseOffset.z * s;
	offset.y = baseOffset.y;
	offset.z = -baseOffset.x * s + baseOffset.z * c;
	vec3d CameraLocation = PlayerLocation + offset;


	float CamOffsetY = 5.0f;
	vec3d CamRotation = vec3d{ MyScratch->Input->GetMovementX(), CamOffsetY, 17.5f};

	CamRotation = PlayerLocation - CameraLocation + vec3d{ MyScratch->Input->GetMovementX(),0,0 };


	CameraSmoothRotation = MyScratch->Lerp(CameraSmoothRotation, CamRotation, (PlayerMovement->Speed / 2.0f) * DeltaTime);
	CameraSmoothLocation = MyScratch->Lerp(CameraSmoothLocation, CameraLocation, (PlayerMovement->Speed/2.0f) * DeltaTime);
	MyScratch->SetCamera_Legacy(CameraSmoothLocation, CameraSmoothRotation);
	//MyScratch->SetCameraFOV(90);
	MyScratch->SetCameraFOV(65);

	//Push zbuffer back to make more "room" for the depth of the scene
	MyScratch->ClearZBufffer();

	//---------------
	//collision + offset
	//---------------
	PlayerLocation = MyScratch->SnapToMesh(PlayerLocation, TerrrainMesh, vec3d{ 0,0,0 });
	//vec3d PlayerLocationMirrored = { PlayerLocation.x,-PlayerLocation.y + 29.0f, PlayerLocation.z };
	vec3d PlayerLocationMirrored = { PlayerLocation.x,-PlayerLocation.y+40.0f, PlayerLocation.z };
	//---------------
	//water:
	//---------------
	MyScratch->ZWriteOn = false; //Depth  off so we can draw as far as possible!
	vec3d WaterLocation= { PlayerLocation.x,0.0f,PlayerLocation.z };
	MyScratch->SetTexture(water, w32, h32);
	MyScratch->TextureDrawOn = true; 
	//Scroll the uvs, and add the water plane location to make water look infinite
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * 0.25f) + (WaterLocation.x * 0.05f),(totalTime * 0.25f) - (WaterLocation.z * 0.05f)}; //Scrolling UV effect. Use this for water later!
	//Wave mesh:
	Mesh wave = MyScratch->WaveMesh(WaterPlaneMesh, totalTime*12.0f, 0.25f);
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//Water second layerFX overlay:
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * -0.25f) + (WaterLocation.x * 0.05f),(totalTime * -0.25f) - (WaterLocation.z * 0.05f) }; //Scrolling UV effect. Use this for water later!
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	
	//Reflection:
	MyScratch->DrawMesh(PlayerMesh, PlayerLocationMirrored, vec3d{ 0,PlayerMovement->GetYaw(),0 }, vec3d{ PlayerScale.x,-PlayerScale.y,PlayerScale.z});
	MyScratch->BlendBuffers(0.25f +abs(sin(totalTime))*0.5f); //blend two water layers
	MyScratch->ClearZBufffer();//don't need this

	//---------------
	//terrain:
	//---------------
	MyScratch->PushBackDepthBuffer(2000); //Give us pleanty of space to draw the terrain!
	MyScratch->ZWriteOn = true;
	MyScratch->UvOffsetGlobal = vec2d{ 0.0f,0.0f };
	MyScratch->SetTexture(overworldTexture, w256, h256);
	MyScratch->TextureDrawOn = true;
	MyScratch->DrawMesh(TerrrainMesh, vec3d{ 0,0,0 }, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//AccumulatedBlur(0.75f); //Blur only BKG if called here!
	//---------------
	//player's shadow:
	//---------------
	MyScratch->MeshColor = RGB_Black;
	MyScratch->MeshColor.a = 128;
	MyScratch->TextureDrawOn = false;
	MyScratch->PushBackDepthBuffer(90);
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation+ vec3d{ 0,0,0 }, vec3d{ 0,PlayerMovement->GetYaw(),0 }, vec3d{ PlayerScale.x,0.1f,PlayerScale.z });
	MyScratch->BlendBuffers(0.5f);
	//---------------
	//player:
	//---------------
	MyScratch->SetTexture(Palette, w64, h64);
	GI_Lighting = MyScratch->Lerp(GI_Lighting, (MyScratch->SnapToMeshTriColor) * 2.5f, 6.0f * DeltaTime);//psudo lighting
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	MyScratch->TextureDrawOn = true;
	
	//edge light
	MyScratch->MeshColor = GI_Lighting * 2.0f;
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation + PlayerOffset - vec3d{ 0.09f,0.09f,0.0f }, vec3d{ 0,PlayerMovement->GetYaw(),0 }, PlayerScale);
	MyScratch->PushBackDepthBuffer(32);
	//end edge light
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	//normal render
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation + PlayerOffset, vec3d{ 0,PlayerMovement->GetYaw(),0 }, PlayerScale);

	//TEXT MAPPED TO PLAYER:
	//vec3d player_2d_loc = MyScratch->Get2DPointFromLastLocation();
	//MyScratch->DrawTextDropShadow(player_2d_loc.x, player_2d_loc.y, RGB_White, "Hello", MyTextSprites, 1.0f);

	//---------------
	//props:
	//---------------
	MyScratch->MeshColor = RGB_White;
	MyScratch->SetTexture(Image03, w16, w16);
	vec3d BoxPropLoc = vec3d{ -53.478f,1.48093f,-29.807f };
	MyScratch->DrawMesh(LoadedMesh2, BoxPropLoc, vec3d{ 0,totalTime,0 }, vec3d{ 1.0f,1.0f,1.0f }); //position copied from blender, but swapped y and -z


	////---------------
	////Scene file objects:
	////---------------
	//MyScratch->ZWriteOn = false;
	//for (int i = 0; i < MeshPropIDs.size(); ++i)
	//{
	//	int objId = MeshPropIDs[i];
	//	MyScratch->DrawMesh(
	//		CurrentScene.Meshes[CurrentScene.scene_objects[objId].model_id],
	//		CurrentScene.scene_objects[objId].pos,
	//		CurrentScene.scene_objects[objId].rot,
	//		CurrentScene.scene_objects[objId].scale,
	//		false);
	//}
	//---------------
	//FX:
	//---------------
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->BrightnessContrastOnBuffer(MyScratch->MainSpace, 0.7f, 2.5f);
	MusicAndFadeIn(DeltaTime);

	//---------------
	//Text
	//---------------
	TextUpdateTick(DeltaTime);
	if (MyScratch->SquaredDistance2D(PlayerLocation, BoxPropLoc) <= 1.0f) {
		RequestedText = "RpgNpcBox";
	}

	/*if (MyScratch->Input->GetFireOneHold())
	{
	//LOAD A  MODEL IN REAL TIME!
		ModelFileParser parser;
		TerrrainMesh = parser.ParseModelFromFile("Assets/DomeModel.txt");
	}*/
}

void GameRPGWorld::RenderMovie()
{
	float totalTime = -1.5f;
	float min = -1.5f;
	float max = 1.5f*4.0f;
	int frames = 120;
	ModelFileParser parser;
	LoadedMesh = parser.ParseModelFromFile("Assets/olexa.txt");
	for (int i = 0; i < frames; ++i)
	{
		totalTime = MyScratch->Lerp(min,max,(float)i/(float)frames);
		MyScratch->MeshColor = { 255,255,255,255 };
		MyScratch->Clear();
		MyScratch->ClearZBufffer();
		int FULL_VAL = (int)(abs(sin(totalTime)) * 255);
		int FULL_VAL2 = (int)(abs(cos(totalTime)) * 255);
		//MyScratch->DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, RGB{ FULL_VAL2 / 2,FULL_VAL,FULL_VAL,255 }, RGB{ 54,32,FULL_VAL2,255 }, RGB{ 0,0,0,255 }, RGB{ 0,0,0,255 });
		MyScratch->DrawMesh(LoadedMesh, vec3d{ 0.0f, -0.1f,-2.0f }, vec3d{ 1.5f, -totalTime ,0 }, vec3d{ 1.2,1.2,1.2 }, false);

		int screenshotIndex = i;

		std::string filename = "C:/tmp/output_" + std::to_string(screenshotIndex) + ".bmp";
		RGB ScanLineColor = { 0,0,16,255 };
		RGB ScanLineColor2 = { 18,0,0,255 };
		WriteBMP_Scaled(
			filename.c_str(),
			MyScratch->MainSpace,
			SCREEN_X,
			SCREEN_Y,
			6,
			ScanLineColor,
			ScanLineColor2
		);
	}

}



void GameRPGWorld::DrawBasics(float DeltaTime)
{
	//Basic example of drawing model to the screen
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	totalTime += DeltaTime;
	MonkeyMesh monkeymesher; //Mesh loading tool
	MyScratch->MeshColor = { 255,255,255,255 };
	MyScratch->SetCameraFOV(122);
	MyScratch->SetCamera_Legacy(vec3d{ 0.0f, -0.0f, -4.0f }, vec3d{ 0.0f,-0.1f, 1.0f });
	//use vec3d to give position, rotation, and scale
	MyScratch->DrawMesh(
		monkeymesher.GetTeapotMesh(),
		vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 1.12f,0.0f,2 },
		vec3d{ 1.0, 1.0, totalTime, },
		vec3d{ 3,3,3 }
	);
	//OPTIONAL (overlay another Teapot) (Untested)
	 //Store and clear the buffer
	MyScratch->MoveMainspaceToExtraBuffer(); MyScratch->Clear();//Clear the scren now that it's backed up 
	MyScratch->DrawMesh(
		monkeymesher.GetTeapotMesh(),
		vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 2.12f,0.0f,2 },
		vec3d{ 1.0, 1.0, totalTime, },
		vec3d{ 3,3,3 }
	);
	MyScratch->AddBuffers();

	//wave FX
	MyScratch->MeshColor = { 0,0,255,255 };
	MyScratch->DrawMesh(
		MyScratch->WaveMesh(
			monkeymesher.GetMonkeyMesh(), 12 * totalTime, 0.15f),
		vec3d{ 0.0f,0.0f,-2.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime * 6.0f), },
		MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f },
			vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))),
		true
	);

	//morph FX
	MyScratch->MeshColor = { 0,222,255,255 };
	Mesh morph = MyScratch->MorphMesh(monkeymesher.GetPyrimidBoxMorph0(), monkeymesher.GetPyrimidBoxMorph(), (sin(totalTime)));
	MyScratch->DrawMesh(morph, vec3d{ 1.8f, 0.0f,-2.0f }, vec3d{ -1.0f,0,0 }, vec3d{ 0.25f,0.25f,0.25f });



	//LOAD FROM FILE STRING
}

void GameRPGWorld::AccumulatedBlur(float strength)
{
	MyScratch->BlendBuffers(BlurBuffer, 1.0f - strength); //smaller number == more blur!
	//For crazy SaGa Frontier style mix
	//MyScratch->AddBuffers(BlurBuffer); //smaller number == more blur!
	MyScratch->CopyBufferToBuffer(MyScratch->MainSpace, BlurBuffer);

}


void GameRPGWorld::TextUpdateTick(float DeltaTime)
{
	if (!EnableTextbox)
	{
		return;
	}
	textBoxProgressTick += 2.5f * DeltaTime;
	TextBoxDraw(Reader->GetStringFromSheetTag(RequestedText));
}

void GameRPGWorld::TextBoxDraw(const char* input)
{
	if (!EnableTextbox)
	{
		return;
	}

	if (input != previous_text)
	{
		textBoxProgressTick = 0.0f;
		previous_text = input;
	}


	if (textBoxProgressTick > 5.0f)
	{
		//Auto-Advanced text until "End" tag found
		if (Reader->HasEventAtIndex(Reader->CurrentLine, "End") == false && Reader->HasEventAtIndex(Reader->CurrentLine + 1, "Start") == false)
		{
			textBoxProgressTick = 0.0f;
			TextBoxDraw(Reader->GetStringFromSheetIndex(Reader->CurrentLine + 1));
		}

		return;
	}


	//TEXT BOX
	int boarder = 2;
	//Fill
	MyScratch->DrawRectangle(
		boarder, SCREEN_Y - (64 + boarder),
		SCREEN_X - (boarder * 2),
		64,
		RGB{ 0,0,255,128 },
		RGB{ 0,222,0,128 },
		RGB{ 0,0,255,0 },
		RGB{ 0,0,111,0 }

	);

	//Top Line
	MyScratch->DrawLine(boarder, SCREEN_Y - (64 + boarder), SCREEN_X - boarder - 1, SCREEN_Y - (64 + boarder), RGB{ 222,222,222,255 });
	//Bottom Line
	MyScratch->DrawLine(boarder, SCREEN_Y - boarder, SCREEN_X - boarder - 1, SCREEN_Y - boarder, RGB{ 222,222,222,255 });
	//Right Side
	MyScratch->DrawLine(SCREEN_X - boarder - 1, SCREEN_Y - (64 + boarder), SCREEN_X - boarder - 1, SCREEN_Y - boarder, RGB{ 222,222,222,255 });
	//Left Side
	MyScratch->DrawLine(boarder, SCREEN_Y - (64 + boarder), boarder, SCREEN_Y - boarder, RGB{ 222,222,222,255 });

	//Coordinates
	const int textX = boarder + 2;
	const int textY = SCREEN_Y - (64);
	//Shadow
	MyScratch->DrawText(textX + 1, textY + 1, { 1, 1, 1, 255, }, input, MyTextSprites, textBoxProgressTick * 0.8f);
	//Text
	MyScratch->DrawText(textX, textY, { 255, 255, 255, 255, }, input, MyTextSprites, textBoxProgressTick * 0.8f);

	//Blinking cursor 
	if (sin(totalTime * 8.0f) > 0.0f)
	{

		MyScratch->DrawText(SCREEN_X - 10, SCREEN_Y - boarder - 7, { 0, 255, 255, 255, }, "|", MyTextSprites, 1.0f);

	}
}


GameRPGWorld::~GameRPGWorld()
{
	delete Reader;
	Reader = nullptr;
	delete MyScratch;
	MyScratch = nullptr;
	delete MyTextSprites;
	MyTextSprites = nullptr;
	delete Image01;
	delete grass;
	delete Image03;
	Image01 = nullptr;
	grass = nullptr;
	Image03 = nullptr;
	delete PlayerMovement;
	PlayerMovement = nullptr;
}


