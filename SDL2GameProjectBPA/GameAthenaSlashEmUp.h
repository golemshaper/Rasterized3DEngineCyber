#pragma once
#include "Game.h"
#include "TextFileReader.h"

class GameAthenaSlashEmUp : public Game
{

public:

	//Current game mode
	int mode = 0;

	//primitive
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text;

	float angle = 0.0f;

	//pools
	int bullet_count = 64;
	vec3d bullets[64];

	//re-usable
	vec3d working_vector = { 0,0,0 };
	RGB working_color = { 0,0,0,0 };
	float working_float = 0.0f;


	//Objects
	TextFileReader Reader;







	void Initialize();
	void Tick(float DeltaTime);
	void TitleScreenTick(float DeltaTime);
	void GameModeTick(float DeltaTime);
	void UnhingedModeTick(float DeltaTime);
	void TextBoxDraw(const char* input);


};

