#pragma once
class DrawScratchSpace;

class Game {
public:
	DrawScratchSpace* MyScratch;
	float angle = 0.0f;
	float totalTime = 0.0f;
	void Initialize();
	void Tick(float DeltaTime);


};