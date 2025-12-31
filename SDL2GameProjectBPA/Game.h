#pragma once
#include "DrawScratchSpace.h"
class TextSprites;



class Game {
public:
	DrawScratchSpace* MyScratch;
	TextSprites* MyTextSprites;
	float typingEffect = -1.0f;
	float angle = 0.0f;
	float totalTime = 0.0f;

	int bullet_count = 64;
	vec3d bullets[64];



	void Initialize();
	void Tick(float DeltaTime);


};