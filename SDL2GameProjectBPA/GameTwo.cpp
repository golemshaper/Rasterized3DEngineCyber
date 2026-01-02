#pragma once
#include "GameTwo.h"
#include "MonkeyMesh.h"
void GameTwo::Tick(float DeltaTime)
{
	//Basic example of drawing model to the screen
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

}