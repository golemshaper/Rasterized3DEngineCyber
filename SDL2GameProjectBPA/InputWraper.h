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
	float GetCameraXAxis();
	float GetCameraYAxis();
	bool GetToggleDepthKey();
	void ResetToggleDepthKey();
	bool GetFireOneHold();
	int GetMouseXYAsValue();
private:
	float lx = 0.0f;
	float ly = 0.0f;

	float cx = 0.0f;
	float cy = 0.0f;


	float previousMouseX;
	float previousMouseY;

	float mouseX;
	float mouseY;

	float mouseJoystickX;
	float mouseJoystickY;


	


	float toggleCooldown = 0.0f;
	bool zDepthDrawOn = false;
	bool inputFireOneHeld = false;

	float Lerp(float a, float b, float c);
};

