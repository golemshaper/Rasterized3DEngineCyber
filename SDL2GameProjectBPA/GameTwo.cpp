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
	TerrrainMesh = parser.ParseFromFile("Assets/TerrainModel.txt"); 
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


	int w = 32, h = 32;
	Image01 = ReadBMP("Assets/HelloBitmap.bmp", w, h);


	int w2 = 16; int h2 = 16;
	Image02 = ReadBMP("Assets/grass.bmp", w2, h2);


	int w3 = 16; int h3 = 16;
	Image03 = ReadBMP("Assets/red_brick.bmp", w3, h3);
	MyScratch->SetTexture(Image03,w3,h3);

	
}
void GameTwo::Tick(float DeltaTime)
{
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->TextureDrawOn = false;
	int w = 16; int h = 16;
	totalTime += DeltaTime;
	animTimer += DeltaTime;

	//DrawBasics(DeltaTime);

	MyScratch->MeshColor = RGB_White;


	//PLAYER MOVEMENT:
	PlayerMovement->ApplyMovement(DeltaTime, MyScratch);
	PlayerMovement->ApplyGroundSnap(TerrrainMesh, MyScratch, vec3d{ 0,-1.3f,0 });
	vec3d PlayerLocation = PlayerMovement->Pos; //CIRCLE: vec3d{ sin(totalTime) * 10.0f,-12.5f,cos(totalTime) * 8.0f };

	//CAMERA:
	//vec3d CameraLocation = vec3d{ 0.0f, -0.9f, -17.5f };
	vec3d CameraLocation = PlayerLocation + vec3d{ 0.0f, -2.9f, -17.5f };
	float CamOffsetY = 8.0f;
	vec3d CamRotation = vec3d{ MyScratch->Input->GetMovementX(), CamOffsetY, 17.5f};
	CameraSmoothRotation = MyScratch->Lerp(CameraSmoothRotation, CamRotation, (PlayerMovement->Speed / 2.0f) * DeltaTime);
	CameraSmoothLocation = MyScratch->Lerp(CameraSmoothLocation, CameraLocation, (PlayerMovement->Speed/2.0f) * DeltaTime);
	MyScratch->SetCamera(CameraSmoothLocation, CameraSmoothRotation);
	MyScratch->SetCameraFOV(90);
	
	
	MyScratch->ClearZBufffer();
	MyScratch->PushBackDepthBuffer(140);


	MyScratch->ZWriteOn = true;
	//collision + offset
	PlayerLocation = MyScratch->SnapToMesh(PlayerLocation, TerrrainMesh, vec3d{ 0,0,0 }) + vec3d{ 0,-1.5f,0 };

	//terrain:
	MyScratch->SetTexture(Image02, w, h);
	MyScratch->TextureDrawOn = true;
	MyScratch->DrawMesh(TerrrainMesh, vec3d{ 0,0,0 }, vec3d{ 0,0,0 }, vec3d{ 1.0f,1.0f,1.0f });
	//shadow
	MyScratch->MeshColor = RGB_Black;
	MyScratch->TextureDrawOn = false;
	MyScratch->PushBackDepthBuffer(90);
	MyScratch->DrawMesh(LoadedMesh2, PlayerLocation - vec3d{ 0,-1.3f,0 }, vec3d{ 0,totalTime,0 }, vec3d{ 1.25f,0.1f,1.25f });
	//box
	MyScratch->SetTexture(Image03, w, h);
	MyScratch->MeshColor = RGB_White;
	MyScratch->TextureDrawOn = true;
	MyScratch->PushBackDepthBuffer(90);
	MyScratch->DrawMesh(LoadedMesh2, PlayerLocation, vec3d{ 0,totalTime,0 }, vec3d{ 1.0f,1.0f,1.0f });
	


	//TODO: In the exporter swap Y and Z! Blender is Z up, but we are Y up like Unity!



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
	delete Image02;
	delete Image03;
	Image01 = nullptr;
	Image02 = nullptr;
	Image03 = nullptr;
	delete PlayerMovement;
	PlayerMovement = nullptr;
}
