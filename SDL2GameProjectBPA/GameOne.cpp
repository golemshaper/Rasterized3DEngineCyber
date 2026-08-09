
#include "GameOne.h"
#include "ModelFileParser.h"
#include "Animator.h"
void GameOne::Initialize()
{
	//Do not add any more complexity to this example project. Make a new one
	MyScratch = new DrawScratchSpace();
	MyScratch->ZWriteOn = true;
	MyScratch->NearClip = 0.0001f;

	MyScratch->Initialize();
	ModelFileParser parser;
	LoadedMesh = parser.ParseModelFromFile("Assets/TeapotModel.txt");
	LoadedMesh2 = parser.ParseModelFromFile("Assets/hello_vertex_color_model.txt");


	//------------------------------
	animator = new Animator();
	
	animator->LoadAnimationFromFile("Assets/hello_animation.txt","MyAnimation");
	animator->InitializeAnim();
}
void GameOne::Tick(float DeltaTime)
{
	MyScratch->Clear();
	MyScratch->ClearZBufffer();
	totalTime += DeltaTime;

	MyScratch->MeshColor = { 222,155,222,255 };
	MyScratch->DrawEdges = cos(totalTime*8) < 0; // the result of the < 0 is used to produce a true or false, so it changes every few frames
	MyScratch->EdgeBrightness = 0.2f;
	MyScratch->DrawVerticies = sin(totalTime*8) < 0; // the result of the < 0 is used to produce a true or false, so it changes every few frames
	
	MyScratch->SetCameraFOV(50);
	MyScratch->SetCamera_Legacy(vec3d{ 0.0f, -0.25f, -4.0f }, vec3d{ 0.0f,-0.1f, 1.0f });
	MyScratch->DrawMesh(LoadedMesh,vec3d{ 0,0,0 },vec3d{ 1.5, 1.0, totalTime},vec3d{ 3,3,3 },true);
	MyScratch->DrawMesh(LoadedMesh2, vec3d{ 0.25f,-1.25f,-0.92f }, vec3d{ 1.5, 1.0, totalTime }, vec3d{ 0.15f,0.15f,0.15f }, true);
	AnimationTest(DeltaTime);
}

void GameOne::AnimationTest(float DeltaTime)
{
	animator->Tick(DeltaTime);
	int curFrame = animator->animationSources[0].currentFrame;
	if (curFrame > 247)
	{
		curFrame = 0;
		animator->animationSources[0].totalTime = 0;
		animator->animationSources[0].currentFrame = 0;
	}
	std::string str = std::to_string(curFrame);
	MyScratch->DrawTextAtPos(4, 4, RGB_White, "ANIM:", MyTextSprites, 1.0f);
	MyScratch->DrawTextAtPos(22, 12, RGB_White, str.c_str(), MyTextSprites, 1.0f);
	int nextFrame = animator->GetNextFrameWithAnimationData(0, 1, curFrame);
	str = std::to_string(nextFrame);
	MyScratch->DrawTextAtPos(82, 12, RGB_White, str.c_str(), MyTextSprites, 1.0f);




	//vec3d debug_vector = animator->InterpolatedVectorByID(animator->animationSources[0].animation[0], 0, 1, 0.0f);
	int vectorIndex = animator->RawFrameDataByFrameValue(0, 1, curFrame).locId;
	int vector2Index = animator->RawFrameDataByFrameValue(0, 1, curFrame+1).locId;
	vec3d debug_vector = animator->animationSources[0].animation[0].vectors[vectorIndex];
	vec3d debug_vector2 = animator->animationSources[0].animation[0].vectors[vector2Index];

	float interpolate_by = (((float)nextFrame * (float)curFrame) / 100.0f) * 0.01f;
	str = std::to_string(interpolate_by);
	MyScratch->DrawTextAtPos(102, 50, RGB_White, str.c_str(), MyTextSprites, 1.0f);

	vec3d debug_vector3 = MyScratch->Lerp(debug_vector, debug_vector2, interpolate_by);
	MyScratch->DrawTextAtPos(22, 24, RGB_White, animator->VectorToString(debug_vector3).c_str(), MyTextSprites, 1.0f);

	//we know know what the current frame is, so we know the next frame.
	//and we know the total time. Work out the in-between value and lerp using that. 
	//the tricky thing is each frame is not individually spaced in the array, but it might not matter...?


}
