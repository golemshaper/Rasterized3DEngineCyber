#include "GameTwo.h"
#include "MonkeyMesh.h"
void GameTwo::Tick(float DeltaTime)
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
		vec3d{3,3,3}
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
	 MyScratch->DrawMesh(morph, vec3d{ 1.8f, 0.0f,-2.0f }, vec3d{-1.0f,0,0 }, vec3d{ 0.25f,0.25f,0.25f });


}