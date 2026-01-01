#pragma once
#include "DrawScratchSpace.h"
class TextSprites;



class Game {
public:
	DrawScratchSpace* MyScratch;
	TextSprites* MyTextSprites;
	
	float totalTime = 0.0f;
	



	virtual void Initialize();
	virtual void Tick(float DeltaTime);

};