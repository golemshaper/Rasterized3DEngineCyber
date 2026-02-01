#include "GameTwo.h"
#include "MonkeyMesh.h"
#include <string>

#include "ModelFileParser.h"
#include "BMPWriter.hpp"
#include "BMPReader.hpp"
#include "ThirdPersonMovement.h"

void GameTwo::Initialize()
{
	MyScratch = new DrawScratchSpace();
	MyScratch->Initialize();
	PlayerMovement = new ThirdPersonMovement();
	PlayerMovement->Pos = vec3d{ 0,-1.3f,0 };
	ModelFileParser parser;

	//LoadedMesh = parser.ParseFromStr(text);
	//MY PC: C:\Users\brian\source\repos\Rasterized3DEngine\SDL2GameProjectBPA\Assets
	LoadedMesh = parser.ParseFromFile("Assets/olexa.txt");
	LoadedMesh2 = parser.ParseFromFile("Assets/CubeTiledUvs.txt");
	PlayerMesh = parser.ParseFromFile("Assets/RPGWarriorRohan.txt");
	TerrrainMesh = parser.ParseFromFile("Assets/TerrainModel.txt"); 
	WaterPlaneMesh = parser.ParseFromFile("Assets/WaterPlaneCutoutCenter.txt");
	//LoadedMesh2 = parser.ParseFromFile("Assets/cube_model.txt");

	
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F0.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F1.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F1.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F2.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F3.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F4.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F5.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F6.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F0.txt"));
	MeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F0.txt"));


	int wGB = 128, hGB = 112;
	Image01 = ReadBMP("Assets/001.bmp", wGB, hGB);


	int w2 = 16; int h2 = 16;
	grass = ReadBMP("Assets/grass.bmp", w2, h2);


	int w32 = 32; int h32 = 32;
	water = ReadBMP("Assets/water.bmp", w32, w32);


	int w3 = 16; int h3 = 16;
	Image03 = ReadBMP("Assets/red_brick.bmp", w3, h3);
	MyScratch->SetTexture(Image03,w3,h3);
	
	int w64 = 64; int h64 = 64;
	Palette = ReadBMP("Assets/BasicPalette.bmp", w64, h64);
	
}
void GameTwo::Tick(float DeltaTime)
{
	//---------------
	//Setup:
	//---------------
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->TextureDrawOn = false;
	int w16 = 16; int h16 = 16;
	int w32 = 32; int h32 = 32;
	int wGB = 128;int hGB = 112;
	int w64 = 64; int h64 = 64;
	totalTime += DeltaTime;
	animTimer += DeltaTime;
	MyScratch->MeshColor = RGB_White;

	//---------------
	//PLAYER MOVEMENT:
	//---------------
	PlayerMovement->ApplyMovement(DeltaTime, MyScratch);
	PlayerMovement->ApplyGroundSnap(TerrrainMesh, MyScratch, vec3d{ 0,-1.3f,0 });
	vec3d PlayerLocation = PlayerMovement->Pos; //CIRCLE: vec3d{ sin(totalTime) * 10.0f,-12.5f,cos(totalTime) * 8.0f };
	//---------------
	//CAMERA:
	//---------------
	vec3d CameraLocation = PlayerLocation + vec3d{ 0.0f, -5.9f, -17.5f };
	float CamOffsetY = 5.0f;
	vec3d CamRotation = vec3d{ MyScratch->Input->GetMovementX(), CamOffsetY, 17.5f};
	CameraSmoothRotation = MyScratch->Lerp(CameraSmoothRotation, CamRotation, (PlayerMovement->Speed / 2.0f) * DeltaTime);
	CameraSmoothLocation = MyScratch->Lerp(CameraSmoothLocation, CameraLocation, (PlayerMovement->Speed/2.0f) * DeltaTime);
	MyScratch->SetCamera(CameraSmoothLocation, CameraSmoothRotation);
	//MyScratch->SetCameraFOV(90);
	MyScratch->SetCameraFOV(65);

	//Push zbuffer back to make more "room" for the depth of the scene
	MyScratch->ClearZBufffer();
	
	//---------------
	//collision + offset
	//---------------
	PlayerLocation = MyScratch->SnapToMesh(PlayerLocation, TerrrainMesh, vec3d{ 0,0,0 });
	//---------------
	//water:
	//---------------
	
	MyScratch->ZWriteOn = false; //Depth  off so we can draw as far as possible!
		//want multi layered water that scrolls at different speeds? render the water twice, push the first ot the back buffer, render again, mix 50% opacity
	vec3d WaterLocation= { PlayerLocation.x,0.0f,PlayerLocation.z };

	MyScratch->SetTexture(water, w32, h32);
	MyScratch->TextureDrawOn = true; 
	//Scroll the uvs, and add the water plane location to make water look infinite
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * 0.25f) + (WaterLocation.x * 0.05f),(totalTime * 0.25f) - (WaterLocation.z * 0.05f)}; //Scrolling UV effect. Use this for water later!
	//standard mesh
	Mesh wave = MyScratch->WaveMesh(WaterPlaneMesh, totalTime*12.0f, 0.25f);
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//Water second layerFX overlay:
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->UvOffsetGlobal = vec2d{ (totalTime * -0.25f) + (WaterLocation.x * 0.05f),(totalTime * -0.25f) - (WaterLocation.z * 0.05f) }; //Scrolling UV effect. Use this for water later!
	MyScratch->DrawMesh(wave, WaterLocation, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	MyScratch->BlendBuffers(abs(sin(totalTime))*0.5f); //blend two water layers
	MyScratch->ClearZBufffer();//don't need this

	//---------------
	//terrain:
	//---------------
	MyScratch->PushBackDepthBuffer(2000); //Give us pleanty of space to draw the terrain!
	MyScratch->ZWriteOn = true;
	MyScratch->UvOffsetGlobal = vec2d{ 0.0f,0.0f };
	MyScratch->SetTexture(grass, w16, h16);
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
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation+ vec3d{ 0,0,0 }, vec3d{ 0,totalTime,0 }, vec3d{ 2.5f,0.1f,2.5f });
	MyScratch->BlendBuffers(0.5f);
	//---------------
	//player:
	//---------------
	MyScratch->SetTexture(Palette, w64, h64);
	GI_Lighting = MyScratch->Lerp(GI_Lighting, (MyScratch->SnapToMeshTriColor) * 2.5f, 6.0f * DeltaTime);//psudo lighting
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	MyScratch->TextureDrawOn = true;
	
	MyScratch->DrawMesh(PlayerMesh, PlayerLocation , vec3d{ 0,totalTime,0 }, vec3d{ 2.5f,2.5f,2.5f });
	//---------------
	//props:
	//---------------
	MyScratch->MeshColor = RGB_White;
	MyScratch->SetTexture(Image01, wGB, hGB);
	MyScratch->DrawMesh(LoadedMesh2, vec3d{ -53.478f,1.48093f,-29.807f }, vec3d{ 0,totalTime,0 }, vec3d{ 1.0f,1.0f,1.0f }); //position copied from blender, but swapped y and -z

	//---------------
	//FX:
	//---------------
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->BrightnessContrastOnBuffer(MyScratch->MainSpace, 0.7f, 2.5f);

}

void GameTwo::RenderMovie()
{
	float totalTime = -1.5f;
	float min = -1.5f;
	float max = 1.5f*4.0f;
	int frames = 120;
	ModelFileParser parser;
	LoadedMesh = parser.ParseFromFile("Assets/olexa.txt");
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

void GameTwo::DrawBasics(float DeltaTime)
{
	//Basic example of drawing model to the screen
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	totalTime += DeltaTime;
	MonkeyMesh monkeymesher; //Mesh loading tool
	MyScratch->MeshColor = { 255,255,255,255 };
	MyScratch->SetCameraFOV(122);
	MyScratch->SetCamera(vec3d{ 0.0f, -0.0f, -4.0f }, vec3d{ 0.0f,-0.1f, 1.0f });
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

void GameTwo::AccumulatedBlur(float strength)
{
	MyScratch->BlendBuffers(BlurBuffer, 1.0f - strength); //smaller number == more blur!
	//For crazy SaGa Frontier style mix
	//MyScratch->AddBuffers(BlurBuffer); //smaller number == more blur!
	MyScratch->CopyBufferToBuffer(MyScratch->MainSpace, BlurBuffer);

}

GameTwo::~GameTwo()
{
	delete Image01;
	delete grass;
	delete Image03;
	Image01 = nullptr;
	grass = nullptr;
	Image03 = nullptr;
	delete PlayerMovement;
	PlayerMovement = nullptr;
}
