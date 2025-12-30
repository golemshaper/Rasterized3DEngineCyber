#pragma once
class DrawScratchSpace;
class TextSprites;

class Game {
public:
	DrawScratchSpace* MyScratch;
	TextSprites* MyTextSprites;

	float angle = 0.0f;
	float totalTime = 0.0f;
	void Initialize();
	void Tick(float DeltaTime);


};