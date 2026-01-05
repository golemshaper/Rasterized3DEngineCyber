#pragma once
#include "Game.h"
#include "TextFileReader.h"

class GameAthenaSlashEmUp : public Game
{

public:
	float GameSpeed = 1.0f;
	//Current game mode
	int mode = 1;



	float demoWait = 4.0f;
	//primitive
	float typingEffect = -1.0f;
	float textBoxProgressTick = 0.0f;
	const char* previous_text;

	float angle = 0.0f;

	//pools
		//athena body
	int bullet_count = 64;
	vec3d bullets[64];
		//arc shots
	int bullet_arcshots_count = 44;
	vec3d bullet_arcshots[44];
	float progress_for_arc_shots=0.0f;

	//actors
	vec3d camLocation = { 0,0,0 };
	vec3d psudoCamLocation= { 0,0,0 };
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
	int fake_exp_until_stats_container_added = 0;

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

	void TickArcShots(vec3d start, vec3d end, float DeltaTime);

	void CircleTransition(float reveal);

	void DrawReticle(int x, int y, int radius, float progress);

	void CameraCollision2D();
};

