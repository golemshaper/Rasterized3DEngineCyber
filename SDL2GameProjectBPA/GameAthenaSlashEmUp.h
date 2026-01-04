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

	//actors
	vec3d player_position = { 0,0,0 };
	vec3d player_move = { 0,0,0 };
	vec3d last_safe_look = { 0,0,0 };
	float player_speed = 4.0f;

	//re-usable
	vec3d working_vector = { 0,0,0 };
	RGB working_color = { 0,0,0,0 };
	float working_float = 0.0f;
	float lightning = 0.0f;
	int lightning_phase = 0;

	//Objects
	TextFileReader Reader;







	void Initialize();
	void Tick(float DeltaTime);
	void TitleScreenTick(float DeltaTime);
	void GameModeTick(float DeltaTime);
	void UnhingedModeTick(float DeltaTime);
	void TextBoxDraw(const char* input);
	void LightningFX(int phase, float progress);

	void MovementUpdate(float DeltaTime);
};

