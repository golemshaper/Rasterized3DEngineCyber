#pragma once
#include "Game.h"
class GameAthenaSlashEmUp : public Game
{

public:

	float typingEffect = -1.0f;
	float angle = 0.0f;
	int bullet_count = 64;
	vec3d bullets[64];


	void Initialize();
	void Tick(float DeltaTime);

};

