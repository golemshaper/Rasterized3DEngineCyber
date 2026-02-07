#include "HelloSceneFile.h"
#include "SceneFileParser.h"
#include <iostream>
void HelloSceneFile::Initialize()
{
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    SceneParser = new SceneFileParser();
    LoadSceneFiles();

}
void HelloSceneFile::LoadSceneFiles()
{
   LoadSceneFiles(SceneLink);
}
void HelloSceneFile::LoadSceneFiles(std::string SceneFileName)
{
    SceneParserObject = SceneParser->ParseSceneFromFile(ScenePath+ SceneFileName, "Assets/Models/");
    //Ideas: Loop through lists of scene objects, find objects with certain gameplay tags, and add them to categories, like
    //for example, and NPC_WALK[] list. Then in the game, you can loop through those and apply movement code to them.
    //TODO: Add custom data dictionary on each scene object for tracking in-game stats, like HP or movement data.
    //One for each data type.
    //You can do things like load the terrain, or other single action commands related to tags here.
    int level_link_index = -1;
    Tag_LevelLink = SceneParserObject.GetTagID("LevelLink");
    for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
    {
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("Player"))
        {
            PlayerID = i; //Do this to have a direct "pointer" to the player. You can perform movement on it before the render loop even happens
            //You can use this concept to put together ID lists of categories of objects
        }
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("Terrain"))
        {
            int MeshId = SceneParserObject.scene_objects[i].model_id;
            TerrainCollider = SceneParserObject.Meshes[MeshId];
        }
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("CamLoc"))
        {
            CameraStart = SceneParserObject.scene_objects[i].pos;
        }
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("CameraAim"))
        {
            CameraEnd = SceneParserObject.scene_objects[i].pos;
        }
        //Get the args of the level link tag, which is the next tag over.
        if (SceneParserObject.scene_objects[i].HasTagIDOutIndex(Tag_LevelLink, level_link_index))
        {
           std::string level_link_arg = SceneParserObject.GetTagStringFromID(SceneParserObject.scene_objects[i].tag_ids[level_link_index + 1]);
           std::cout << level_link_arg <<" was found in the level link";
        }
       
    }
    Tag_Hidden = SceneParserObject.GetTagID("Hidden");
    Tag_Spin = SceneParserObject.GetTagID("Spin");

}
void HelloSceneFile::Tick(float DeltaTime)
{
    //TODO: GET TAGS WORKING, ADD TAG INTERFACE TO ENGINE TOOLS IN BLENDER
    MyScratch->Input->Tick(DeltaTime);
    totalTime += DeltaTime;
    MyScratch->ZWriteOn = true;
    MyScratch->Clear(RGB{ 122,2,43 });
    MyScratch->ClearZBufffer();
    MyScratch->SetCameraFOV(90);
    
    MyScratch->MeshColor = RGB{ 255,255,255,255 };
   // MyScratch->SetCamera(vec3d{ 0.0f, -4.8f, -10.1f }, vec3d{ 0.0f,0.4f, 1.0f });
    MyScratch->SetCamera(CameraStart, CameraEnd);
    //Draw Calls

    for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
    {
        //IDs
        int MeshId = SceneParserObject.scene_objects[i].model_id;
        if (SceneParserObject.scene_objects[i].visible == false) { continue; }
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
        //Tags
        if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Hidden))
        {
            continue;
        }
        if (SceneParserObject.scene_objects[i].HasTagByID(Tag_Spin))
        {
            SceneParserObject.scene_objects[i].rot.y = totalTime;
        }
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("Player"))
        {
            SceneParserObject.scene_objects[i].pos = MyScratch->SnapToMesh(SceneParserObject.scene_objects[i].pos , TerrainCollider, vec3d{0,0,0}) - vec3d{ 0,0.35f,0 };
        }
        //LevelLink---
        int level_link_index = -1;
        if (totalTime >= 5.0f && SceneParserObject.scene_objects[i].HasTagIDOutIndex(Tag_LevelLink, level_link_index))
        {
            //After 5 seconds, we get check if we have a level link tag, and then we also get the neighboring tag, which stores the name of our level. 
            //we then load and replace our current scene with this new level. In a game, we'd do a collision check instead of a timer usually
            totalTime = 0.0f;
            std::string level_link_arg = SceneParserObject.GetTagStringFromID(SceneParserObject.scene_objects[i].tag_ids[level_link_index + 1]);
            std::cout << level_link_arg;
            LoadSceneFiles(level_link_arg);
            return;
        }
        //---
        if (SceneParserObject.scene_objects[i].HasTagStringCompare("Shadow"))
        {
            bool texOn = MyScratch->TextureDrawOn; 
            MyScratch->TextureDrawOn = false;
            MyScratch->ZOffset = 90;
            MyScratch->MeshColor = RGB{ 32,32,32,128 };
            MyScratch->DrawMesh(
                SceneParserObject.Meshes[MeshId],
                SceneParserObject.scene_objects[i].pos + vec3d{0,0.001f,0},
                SceneParserObject.scene_objects[i].rot,
                vec3d{ SceneParserObject.scene_objects[i].scale.x,0.02f,SceneParserObject.scene_objects[i].scale.z},
                false
            );
            MyScratch->TextureDrawOn = texOn;
            MyScratch->MeshColor = RGB{ 255,255,255,255 };
        }
        //MESH
        MyScratch->DrawMesh(
            SceneParserObject.Meshes[MeshId], 
            SceneParserObject.scene_objects[i].pos,
            SceneParserObject.scene_objects[i].rot,
            SceneParserObject.scene_objects[i].scale,
            false
        );
        
    }
    //RELOAD CALL
    if (reload_scene_limit_once == false && MyScratch->Input->GetFireOneHold())
    {
        LoadSceneFiles();
        reload_scene_limit_once = true;
    }
    if (!MyScratch->Input->GetFireOneHold())
    {
        reload_scene_limit_once = false;
    }
}
