#pragma once
#include "Game.h"
class Animator;



class GameOne : public Game
{
public:
	void Initialize();
	void Tick(float DeltaTime);
	void AnimationTest(float DeltaTime);
	Mesh LoadedMesh;
	Mesh LoadedMesh2;
	Animator* animator;
};

