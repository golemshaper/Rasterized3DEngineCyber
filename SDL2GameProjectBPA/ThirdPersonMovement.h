#pragma once
#include "DrawScratchSpace.h"
class ThirdPersonMovement
{
public: 
	vec3d Pos = { 0,0,0 };
	vec3d LastSafePos = { 0,0,0 };
	vec3d Rot = { 0,0,0 };
	float Speed = 12.0f;
	void ApplyMovement(float DeltaTime, DrawScratchSpace* MyScratch);
	void ApplyGroundSnap(const Mesh& mesh, DrawScratchSpace* MyScratch,vec3d offsetMe = vec3d{ 0,-1.5f,0 });

};

