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
	PlayerMovement(DeltaTime);

	MyScratch->Clear(RGB_Blue);
	MyScratch->ClearZBufffer();
	MyScratch->DrawVerticies = false;
	DrawSkyboxMesh();
	totalTime += DeltaTime;
	//MyScratch->DrawVerticies = sin(totalTime * 8.0f) < 0.0f;
	//MyScratch->DrawEdges = true;
	Rail_BKG_Draw(DeltaTime);
	DrawPlayer(DeltaTime);
	DrawReticle(DeltaTime);
	
//	DrawBlackBars();
}
void GameAthenaRailShmup::Reload()
{
	//TODO: Add a button in the engine tools that writes the current frame of the Blender playhead to a file.
	//when reloading get the data from that file, and pass it to ResetAnimation
	
	//TODO: Reparse the animation before reloading it, so that we can reload any keyframes that were tweaked.
	player.firstRun = true;
	CameraAnimator->ResetAnimation(0);
	CameraAnimator->LoadAnimationFromFile("Assets/Animations/CameraAnim_RailShmup_LvlOne.txt", "lvl1");
};

void GameAthenaRailShmup::DrawSkyboxMesh()
{
	//TODO: Draw correct texture here! We overwrode it when drawing the cursor! 
	int firstFrame=138;
	
	AnimTransform camLoc = CameraAnimator->GetAnimatedTransform(0, 0, firstFrame, 222);
	//obj 2
	AnimTransform camTarget = CameraAnimator->GetAnimatedTransform(0, 1, firstFrame, 222);
	MyScratch->SetCameraFOV(75);

	vec3d modCameraBase = camLoc.loc * vec3d{ 1,1 + (0.02f * sin(totalTime)),1 };
	vec3d playerLocMod = vec3d{ player.local_position.x,player.local_position.y,0.0f };
	modCameraBase = MyScratch->Lerp(modCameraBase, modCameraBase - player.local_position, 0.25f);


	MyScratch->SetCamera(modCameraBase - vec3d{ MouseX ,MouseY,0 }, (camTarget.loc * vec3d{ 1,1,1 }) + vec3d{ MouseX ,MouseY,0});


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

	player.cameraLoc = camTarget.loc; //store camera location info in the player.
	vec3d modTarget = camTarget.loc;
	vec3d modCameraBase = camLoc.loc;

	/*modCameraBase = (modCameraBase+(modCameraBase - player.local_position))/2;
	modTarget = modTarget + player.local_position;*/
	//modCameraBase = (((modCameraBase - player.local_position) + modCameraBase) + modCameraBase) / 3.0f;
	modCameraBase = MyScratch->Lerp(modCameraBase, modCameraBase - player.local_position, 0.25f);
	/*modTarget = modTarget + player.local_position;*/


	MyScratch->SetCamera((modCameraBase * vec3d{ 1,1,1 }) - vec3d{ MouseX ,MouseY,0 },(modTarget * vec3d{ 1,1,1 }) + vec3d{ MouseX ,MouseY,0 });
	

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
void GameAthenaRailShmup::PlayerMovement(float DeltaTime)
{
	//get input
	vec3d inputData = MyScratch->GetMovementInput();
	vec3d inputData2 = vec3d{ inputData.x,inputData.z,inputData.y };
	player.storeInputVector = inputData2;
	vec3d prevPos = player.local_position;
	//movement
	player.local_position = player.local_position + (inputData2 * player.speed * DeltaTime);
	//restrict zone
	if ((MyScratch->SquaredDistance2D2(player.cameraLoc, player.local_position)) > player.maxDist * player.maxDist)
	{
		player.local_position = prevPos;
		if (player.firstRun)
		{
			player.firstRun = false;
			player.local_position = vec3d{ player.cameraLoc.x,player.cameraLoc.y,player.local_position.z };
		}
	}
	//display location/collision location
	player.perceptual_location = player.cameraLoc + player.local_position + vec3d{ -1.5f,2.5f,0.0f };

}
void GameAthenaRailShmup::DrawPlayer(float DeltaTime)
{
	MyScratch->DrawMesh(player.Mesh_PlayerIdle, player.perceptual_location, vec3d{0,0,0});
}
void GameAthenaRailShmup::DrawReticle(float DeltaTime)
{
	MyScratch->ClearZBufffer();

	vec3d cursorInput = vec3d{ (float)(int)player.storeInputVector.x,	(float)(int)player.storeInputVector.y,(float)(int)player.storeInputVector .z};
	cursorInput.x = cursorInput.x * 4.10f;
	cursorInput.y = cursorInput.y * 3.65f;
	cursorInput.y = cursorInput.y * -1;
	MyScratch->SetCameraFOV(100.0f);
	MyScratch->SetCamera(vec3d{ 0,0,-4 } + cursorInput, vec3d{ 0,0,100 });
	MyScratch->SetTexture(
		SceneParserObject.TexturePacks[player.CursorTextureId].TextureData,
		SceneParserObject.TexturePacks[player.CursorTextureId].width,
		SceneParserObject.TexturePacks[player.CursorTextureId].height
	);
	MyScratch->DrawMesh(player.Mesh_Cursor, vec3d{ 0,0,0 }, vec3d{ 0,0,totalTime*2.0f});

	//MyScratch->Draw3DLine(player.perceptual_location, player.cameraLoc, RGB_White);
	//Draw grid:
	if (sin(totalTime*12.0f) > 0.5f)
	{
		const int divisions = 3;
		const int spacingY = SCREEN_Y / divisions;
		const int spacingX = SCREEN_X / divisions;

		for (int i = 1; i < divisions; ++i)
		{
			// Horizontal lines
			int y = spacingY * i;
			MyScratch->DrawLine(0, y, SCREEN_X - 1, y, RGB{ 255,64,255,100 });
			// Vertical lines
			int x = spacingX * i;
			MyScratch->DrawLine(x, 0, x, SCREEN_Y - 1, RGB{ 255,64,255,100 });
		}
	}
	

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
	Tag_Cursor = SceneParserObject.GetTagID("Cursor");
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
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Player))
		{
			//hide this player placeholder. Now that it's loaded in the player, we'll use that to draw the data.
			//do the same for any other player frames we load in
			SceneParserObject.scene_objects[i].visible = false;
			player.Mesh_PlayerIdle = SceneParserObject.Meshes[SceneParserObject.scene_objects[i].model_id];
		}
		if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Cursor))
		{
			//store and hide cursor mesh
			SceneParserObject.scene_objects[i].visible = false;
			player.CursorTextureId = SceneParserObject.scene_objects[i].texture_id;
			player.Mesh_Cursor = SceneParserObject.Meshes[SceneParserObject.scene_objects[i].model_id];
		}
	}
};