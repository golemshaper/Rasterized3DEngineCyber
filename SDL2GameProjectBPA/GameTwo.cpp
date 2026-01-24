#include "GameTwo.h"
#include "MonkeyMesh.h"
#include <string>

#include "ModelFileParser.h"
#include "BMPWriter.hpp"
#include "BMPReader.hpp"

void GameTwo::Initialize()
{
	MyScratch = new DrawScratchSpace();
	MyScratch->Initialize();


	ModelFileParser parser;

	//LoadedMesh = parser.ParseFromStr(text);
	//MY PC: C:\Users\brian\source\repos\Rasterized3DEngine\SDL2GameProjectBPA\Assets
	LoadedMesh = parser.ParseFromFile("Assets/olexa.txt");
	LoadedMesh2 = parser.ParseFromFile("Assets/UvHelloWorldCube.txt");
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


	int w2 = 128; int h2 = 112;
	Image02 = ReadBMP("Assets/011.bmp", w2, h2);
	Image03 = ReadBMP("Assets/001.bmp", w2, h2);
	MyScratch->SetTexture(Image03,w2,h2);

	
}
void GameTwo::Tick(float DeltaTime)
{
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	MyScratch->TextureDrawOn = false;

	totalTime += DeltaTime;
	animTimer += DeltaTime;

	int FULL_VAL = (int)(abs(sin(totalTime)) * 255);
	int FULL_VAL2 = (int)(abs(cos(totalTime))*255);
	MyScratch->DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, RGB{ FULL_VAL2/2,FULL_VAL,FULL_VAL,255 }, RGB{ 54,32,FULL_VAL2,255 }, RGB{ 0,0,0,255 }, RGB{ 0,0,0,255 });
	//DrawBasics(DeltaTime);

	MyScratch->MeshColor = { 255,255,255,255 };

	MyScratch->DrawUnlit = true;
	MyScratch->SetCamera(vec3d{ 0.0f, -0.9f, -7.5f }, vec3d{ 0.0f, 0.1f, 2.0f });
	MyScratch->SetCameraFOV(50);
	//MyScratch->DrawMesh(LoadedMesh, vec3d{ 0.0f, -0.1f,-2.0f }, vec3d{ 1.5f,totalTime * 2.0f,0 }, vec3d{ 1.2,1.2,1.2 }, true);
	MyScratch->HighlightBrightness = 2.0f;
	MyScratch->DrawMesh(LoadedMesh, vec3d{0.0f, -0.1f,-2.0f}, vec3d{1.5f, totalTime*2.0f ,0}, vec3d{1.2,1.2,1.2},true);
	MyScratch->DrawUnlit = false;


	if (animTimer >= 0.05f)
	{
		animTimer = 0.0f;
		cur_frame++;
		if (cur_frame == 10)cur_frame = 0;
	}
	MyScratch->MeshColor = { FULL_VAL,FULL_VAL2,0,255 };

	//MyScratch->DrawMesh(MeshSequence[cur_frame], vec3d{ 1.0f, -0.1f,-2.0f }, vec3d{ 1.5f,0.0f,0 }, vec3d{ 1.2,1.2,1.2 }, true);
	//MyScratch->DrawMesh(MeshSequence[cur_frame], vec3d{-1.0f, -0.1f,-2.0f}, vec3d{1.5f,totalTime * 2.0f,0}, vec3d{1.2,1.2,1.2}, true);
	
	//darken image a bit
	MyScratch->DrawSquare(0,0, 512, RGB{ 0,0,0,200 });
	//img 1
	int w = 32, h = 32;
	Sprite MySprite = Sprite{ Image01, w,h };
	MyScratch->DrawSprite(64, 64, MySprite);

	//img 2
	int w2 = 128, h2 = 112;
	Sprite MySprite2 = Sprite{ Image02, w2,h2 };
	MyScratch->DrawSprite(0, 96, MySprite2);

	//img 3
	Sprite MySprite3 = Sprite{ Image03, w2,h2 };
	MyScratch->DrawSprite(128, 96, MySprite3);
	//darken image a bit
	MyScratch->DrawSquare(0, 0, 512, RGB{ 0,0,0,164 });
	//draw box
	MyScratch->MeshColor = RGB{ 255,255,255,255 };
	MonkeyMesh monkeymesher;
	MyScratch->TextureBuffer = Image03;
	MyScratch->TextureDrawOn = true;
	MyScratch->DrawMesh(LoadedMesh2,vec3d{0,0,0},vec3d{0,totalTime,0},vec3d{1.5f,1.5f,1.5f});





	//AccumulatedBlur(0.5f);
	if (!screenshot_fire_once)
	{

		//RENDER MOVIE::   
		//RenderMovie();


		screenshot_fire_once = true;
		//RGB ScanLineColor = { 0,0,16,255 };
		//RGB ScanLineColor2 = { 18,0,0,255 };
		////TODO: Make a turntable runder function and render out an image sequence for this animation!
		////WriteBMP_ScaledWithScanlines("C:/tmp/output.bmp", MyScratch->MainSpace, SCREEN_X, SCREEN_Y,6, ScanLineColor, ScanLineColor2);

		//int screenshotIndex = 12; // whatever you want

		//std::string filename = "C:/tmp/output_" + std::to_string(screenshotIndex) + ".bmp";

		//WriteBMP_ScaledWithScanlines(
		//	filename.c_str(),
		//	MyScratch->MainSpace,
		//	SCREEN_X,
		//	SCREEN_Y,
		//	6,
		//	ScanLineColor,
		//	ScanLineColor2
		//);

	}
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