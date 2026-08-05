#pragma once
#include "DrawScratchSpace.h"
#include <string>

//lvl 2
struct Frame
{
	int frame;
	vec3d loc;
	vec3d rot;
	vec3d scale;
};
//lvl 3
struct Animation
{
	Frame* frames;
};
//lvl 1
struct AnimatedObject
{
	int objID;
	std::string animName;
	Animation* animation;
	int currentFrame;
	int fps = 24;
};
//lvl 0 deep
class Animator
{
	
	AnimatedObject* animatedObjects;
	float totalTime = 0.0f;
public:
	void Tick(float DeltaTime);
	void LoadAnimationFromString(const std::string& str, const std::string& animName);
private:
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::string trim(const std::string& s);
};

