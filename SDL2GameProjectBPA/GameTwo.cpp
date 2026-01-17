#include "GameTwo.h"
#include "MonkeyMesh.h"
#include <string>

#include "ModelFileParser.h"

void GameTwo::Initialize()
{
	MyScratch = new DrawScratchSpace();
	MyScratch->Initialize();


	std::string text = R"(
vectors:

 -0.440443 -0.440443 -0.440443

 -0.440443 -0.440443 0.440443

 -0.440443 0.440443 -0.440443

 -0.440443 0.440443 0.440443

 0.440443 -0.440443 -0.440443

 0.440443 -0.440443 0.440443

 0.440443 0.440443 -0.440443

 0.440443 0.440443 0.440443

 -1.408134 0.000000 0.000000

 0.000000 1.408134 0.000000

 1.408134 0.000000 0.000000

 0.000000 -1.408134 0.000000

 0.000000 0.000000 -1.408134

 0.000000 0.000000 1.408134

tris:

 1 8 0

 3 8 1

 2 8 3

 0 8 2

 3 9 2

 7 9 3

 6 9 7

 2 9 6

 7 10 6

 5 10 7

 4 10 5

 6 10 4

 5 11 4

 1 11 5

 0 11 1

 4 11 0

 6 12 2

 4 12 6

 0 12 4

 2 12 0

 3 13 7

 1 13 3

 5 13 1

 7 13 5

)";

	ModelFileParser parser;

	//LoadedMesh = parser.ParseFromStr(text);
	//MY PC: C:\Users\brian\source\repos\Rasterized3DEngine\SDL2GameProjectBPA\Assets
	LoadedMesh = parser.ParseFromFile("Assets/hello_vertex_color_model.txt");


	
}
void GameTwo::Tick(float DeltaTime)
{
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
	totalTime += DeltaTime;
	


	//DrawBasics(DeltaTime);

	MyScratch->MeshColor = { 255,255,255,255 };

	MyScratch->SetCameraFOV(90);
	MyScratch->SetCamera(vec3d{ 0.0f, -0.0f, -6.0f }, vec3d{ 0.0f, 0.0f, 2.0f });

	MyScratch->DrawMesh(LoadedMesh, vec3d{0.0f, -0.1f,-2.0f}, vec3d{1.5f,totalTime*2.0f,0}, vec3d{1,1,1});
	
	
}

void GameTwo::DrawBasics(float DeltaTime)
{
	//Basic example of drawing model to the screen
	MyScratch->ZWriteOn = false;
	MyScratch->Clear();
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
