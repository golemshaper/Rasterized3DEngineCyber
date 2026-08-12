#pragma once
#include "DrawScratchSpace.h"
#include <string>
#include <unordered_map>

//lvl 2
struct Frame
{
	int objId;
	int frame;
	/*vec3d loc;
	vec3d rot;
	vec3d scale;*/
	int locId;
	int rotId;
	int scaleId;


};
//lvl 3
struct Animation
{
	//animation contains frames for all objects that are part of the animation. Needs to be filtered by the ID.
	//object frames should be stored together, so don't worry about it too much.

	//vec3d* vectors;

	//pre-alocated arrays would probably be nicer

	std::vector<vec3d> vectors;
	std::vector<Frame> frames;

};
//lvl 1
struct AnimationSource
{

	int objID=0;
	std::string animName;
	Animation* animation;
	//pre-alocated arrays would probably be nicer
	std::vector<std::string> objectNames;
	std::unordered_map<std::string, int> objectMap;

	float totalTime = 0.0f;
	float accumulator = 0.0f;
	int currentFrame=0;
	int fps = 24;
};
//lvl 0 deep
class Animator
{
public:
	//pre-alocated arrays would probably be nicer
	std::vector<AnimationSource> animationSources;
	
public:
	void InitializeAnim();
	void Tick(float DeltaTime);
	void LoadAnimationFromString(const std::string& str, const std::string& animName);
	void LoadAnimationFromFile(const std::string path, const std::string& animName);

	vec3d InterpolatedVectorByID(Animation anim, int a, int b, float c);
	Frame RawFrameDataByFrameValue(int sourceIndex,int curObj,int curFrame);
	int GetNextFrameWithAnimationData(int sourceIndex, int curObj, int curFrame);
	int GetPrevFrameWithAnimationData(int sourceIndex, int curObj, int curFrame);
	float GetBetweenFrameTime(float currentFrame, float lastKey, float nextKey, float totalFrames);
	std::string VectorIdToString(Animation anim, int a);
	std::string VectorToString(vec3d a);
private:
	std::vector<std::string> SplitByChar(const std::string& str, char c);
	std::string trim(const std::string& s);

	
};

