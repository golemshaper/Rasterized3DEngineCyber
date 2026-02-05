#include "HelloSceneFile.h"
#include "SceneFileParser.h"
void HelloSceneFile::Initialize()
{
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    SceneParser = new SceneFileParser();

    SceneParserObject = SceneParser->ParseSceneFromFile("Assets/Scenes/HELLO_SCENE_FILE.txt","Assets/Models/");
}
void HelloSceneFile::Tick(float DeltaTime)
{
    MyScratch->Input->Tick(DeltaTime);
    totalTime += DeltaTime;
    MyScratch->ZWriteOn = true;
    MyScratch->Clear(RGB{ 122,2,43 });
    MyScratch->ClearZBufffer();
    MyScratch->SetCameraFOV(90);
    MyScratch->MeshColor = RGB{ 90,90,90,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -4.8f, -10.1f }, vec3d{ 0.0f,0.4f, 1.0f });
    MyScratch->PushBackDepthBuffer(1000);
    //Draw Calls
    for (int i = 0; i < SceneParserObject.scene_objects.size(); i++)
    {
        int MeshId = SceneParserObject.scene_objects[i].model_id;
        MyScratch->DrawMesh(
            SceneParserObject.Meshes[MeshId], 
            SceneParserObject.scene_objects[i].pos, 
            SceneParserObject.scene_objects[i].rot, 
            SceneParserObject.scene_objects[i].scale
        );
    }
    //RELOAD CALL
    if (reload_scene_limit_once == false && MyScratch->Input->GetFireOneHold())
    {
        SceneParserObject = SceneParser->ParseSceneFromFile("Assets/Scenes/HELLO_SCENE_FILE.txt", "Assets/Models/");
        reload_scene_limit_once = true;
    }
    if (!MyScratch->Input->GetFireOneHold())
    {
        reload_scene_limit_once = false;
    }
}
