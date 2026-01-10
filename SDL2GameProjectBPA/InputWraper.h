#pragma once
#include <SDL3/SDL_stdinc.h>

class InputWraper
{
public:

	Uint32 lastInput;
	void Prepare();
	void Tick(float DeltaTime);
	float GetMovementX();
	float GetMovementY();
	bool GetToggleDepthKey();

private:
	float lx = 0.0f;
	float ly = 0.0f;
	float toggleCooldown = 0.0f;
	bool zDepthDrawOn = false;
	float Lerp(float a, float b, float c);
};

