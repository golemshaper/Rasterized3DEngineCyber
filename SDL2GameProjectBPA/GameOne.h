#pragma once
#include "Game.h"
class Animator;



class GameOne : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void AnimationTest(float DeltaTime);
	void AnimationTest2(float DeltaTime);
	void CameraAnimTest3(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	Mesh EnvironmentMesh;
	Animator* animator;
	Animator* CameraAnimator;
	RGB* Texture;
	float delay = 9.0f;
};

