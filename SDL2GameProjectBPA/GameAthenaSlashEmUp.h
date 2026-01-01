#pragma once
#include "Game.h"
#include "TextFileReader.h"

class GameAthenaSlashEmUp : public Game
{

public:

	int mode = 1;
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text;

	float angle = 0.0f;
	int bullet_count = 64;
	vec3d bullets[64];

	TextFileReader Reader;


	void Initialize();
	void Tick(float DeltaTime);
	void TitleScreenTick(float DeltaTime);
	void GameModeTick(float DeltaTime);
	void UnhingedModeTick(float DeltaTime);
	void TextBoxDraw(const char* input);


};

