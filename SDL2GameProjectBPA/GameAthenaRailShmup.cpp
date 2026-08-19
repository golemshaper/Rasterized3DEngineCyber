#include "GameAthenaRailShmup.h"
#include "ModelFileParser.h"
#include "Animator.h"
#include "BMPReader.hpp"
#include "SceneFileParser.h"

void GameAthenaRailShmup::Initialize()
{
	MyScratch = new DrawScratchSpace();
	CameraAnimator = new Animator();
	
	MyScratch->Initialize();

	CameraAnimator->LoadAnimationFromFile("Assets/Animations/CameraAnim_RailShmup_LvlOne.txt", "lvl1");
	CameraAnimator->InitializeAnim();
	LoadScene(SceneLink);

	

};
void GameAthenaRailShmup::Tick(float DeltaTime)
{
	//-------------------------------------------------
	if (MyScratch->Input->GetToggleDepthKey())
	{
		MyScratch->Input->ResetToggleDepthKey();
		Reload();
		return;
	}
	//-------------------------------------------------
	/*if (wait > 0.0f)
	{
		wait -= DeltaTime;
		return;
	}*/
	//-------------------------------------------------
	MyScratch->Input->Tick(DeltaTime);
	MouseX = MyScratch->Input->mouseX * 0.0001f;
	MouseY = MyScratch->Input->mouseY * -0.0001f;


	MyScratch->Clear(RGB_Blue);
	MyScratch->ClearZBufffer();
	MyScratch->DrawVerticies = false;
	DrawSkyboxMesh();
	totalTime += DeltaTime;
	//MyScratch->DrawVerticies = sin(totalTime * 8.0f) < 0.0f;
	//MyScratch->DrawEdges = true;
	Rail_BKG_Draw(DeltaTime);


	
//	DrawBlackBars();
}
void GameAthenaRailShmup::Reload()
{
	//TODO: Add a button in the engine tools that writes the current frame of the Blender playhead to a file.
	//when reloading get the data from that file, and pass it to ResetAnimation
	
	//TODO: Reparse the animation before reloading it, so that we can reload any keyframes that were tweaked.
	
	CameraAnimator->ResetAnimation(0);
	CameraAnimator->LoadAnimationFromFile("Assets/Animations/CameraAnim_RailShmup_LvlOne.txt", "lvl1");
};

void GameAthenaRailShmup::DrawSkyboxMesh()
{
	int firstFrame=138;
	
	AnimTransform camLoc = CameraAnimator->GetAnimatedTransform(0, 0, firstFrame, 222);
	//obj 2
	AnimTransform camTarget = CameraAnimator->GetAnimatedTransform(0, 1, firstFrame, 222);
	MyScratch->SetCameraFOV(75);

	
	
	MyScratch->SetCamera((camLoc.loc * vec3d{ 1,1 + (0.02f * sin(totalTime)),1 })- vec3d{ MouseX ,MouseY,0 }, (camTarget.loc * vec3d{ 1,1,1 }) + vec3d{ MouseX ,MouseY,0});


	MyScratch->DrawMesh(SkyboxMesh, SkyboxLOC, vec3d{ 0,0,0 });
	MyScratch->FilterScreenColorAdd(RGB_Blue);
	MyScratch->ClearZBufffer();
}

void GameAthenaRailShmup::Rail_BKG_Draw(float DeltaTime)
{
	MyScratch->FarClip = 90.0f;
	MyScratch->NearClip = 0.5f;
	int EndOfAnimation = 1918;
	

	CameraAnimator->Tick(DeltaTime);

	MyScratch->UseGouraudShading = true;
	MyScratch->ZWriteOn = true;
	MyScratch->UseDepthFog = true;


	int curFrame = CameraAnimator->animationSources[0].currentFrame;
	//obj 1
	AnimTransform camLoc = CameraAnimator->GetAnimatedTransform(0, 0, curFrame, EndOfAnimation);
	//obj 2
	AnimTransform camTarget = CameraAnimator->GetAnimatedTransform(0, 1, curFrame, EndOfAnimation);
	MyScratch->SetCameraFOV(75);

	
	MyScratch->SetCamera((camLoc.loc * vec3d{ 1,1,1 }) - vec3d{ MouseX ,MouseY,0 },(camTarget.loc * vec3d{ 1,1,1 }) + vec3d{ MouseX ,MouseY,0 });
	

	//TODO Animate the light in the scene file instead
	/*MyScratch->LightDir = MyScratch->LookAtRotation(camLoc.loc + vec3d{ 0,50,0 }, camTarget.loc * vec3d{ 32,-200,32 });
	MyScratch->LightDir = MyScratch->LightDir * -1.0f;*/


	RGB GI_Lighting = { 455,455,455,255 };
	MyScratch->MeshColor = GI_Lighting; //psudo lighting
	//MyScratch->DrawMesh(EnvironmentMesh, vec3d{ 0,0,0 }, vec3d{ 0,0,0 }, vec3d{ 1,1,1 }, true);



	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		//MODEL
		if (SceneParserObject.scene_objects[i].visible == false)
		{
			continue; //Hidden
		}
		
		int MeshId = SceneParserObject.scene_objects[i].model_id;
		int TextureID = SceneParserObject.scene_objects[i].texture_id;
		//TEXTURE
		if (TextureID != -1 && SceneParserObject.TexturePacks.size() >= TextureID)
		{
			MyScratch->TextureDrawOn = true;
			MyScratch->SetTexture(
				SceneParserObject.TexturePacks[TextureID].TextureData,
				SceneParserObject.TexturePacks[TextureID].width,
				SceneParserObject.TexturePacks[TextureID].height
			);
		}
		else
		{
			MyScratch->TextureDrawOn = false;
		}
		//DRAW
		MyScratch->DrawMesh(
			SceneParserObject.Meshes[MeshId],
			SceneParserObject.scene_objects[i].pos,
			SceneParserObject.scene_objects[i].rot,
			SceneParserObject.scene_objects[i].scale,
			false
		);
	}
	//Draw fog FX:
	MyScratch->MoveMainspaceToExtraBuffer();
	MyScratch->DrawZBuffferMaskSky();
	MyScratch->InvertBuffer();
	MyScratch->BrightnessContrastOnBuffer(MyScratch->MainSpace, -5.0f, 1.5f);
	MyScratch->FilterScreenColor(RGB_Blue);
	MyScratch->AddBuffers();

}
void GameAthenaRailShmup::DrawBlackBars()
{
	MyScratch->DrawRectangle(0, SCREEN_Y - 16, SCREEN_X, 16, RGB_Black);
	//used to hide popping triangles
};
void GameAthenaRailShmup::LoadScene(std::string SceneFileName)
{
	SceneParserObject = SceneParser->ParseSceneFromFile(ScenePath + SceneFileName, "Assets/Models/RailShooter/", "Assets/");
	Tag_Hidden = SceneParserObject.GetTagID("Hidden");
	Tag_Player = SceneParserObject.GetTagID("Player");
	Tag_SkyboxMesh = SceneParserObject.GetTagID("SkyboxMesh");
	

	for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
	{
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Hidden))
		{
			SceneParserObject.scene_objects[i].visible = false;
		}
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_SkyboxMesh))
		{
			//for now I have the first terrain chunk loaded as the skybox mesh
			SkyboxMesh = SceneParserObject.Meshes[SceneParserObject.scene_objects[i].model_id];
			SkyboxLOC = SceneParserObject.scene_objects[i].pos;
		}
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_SkyboxMesh))
		{
			player.Mesh_PlayerIdle = SceneParserObject.Meshes[SceneParserObject.scene_objects[i].model_id];
		}
	}
};