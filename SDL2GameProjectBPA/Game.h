#pragma once
class DrawScratchSpace;
class TextSprites;

class Game {
public:
	DrawScratchSpace* MyScratch;
	TextSprites* MyTextSprites;
	float typingEffect = -1.0f;
	float angle = 0.0f;
	float totalTime = 0.0f;
	void Initialize();
	void Tick(float DeltaTime);


};