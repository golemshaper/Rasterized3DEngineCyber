#include "HelloSceneFile.h"
#include "SceneFileParser.h"
void HelloSceneFile::Initialize()
{
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    SceneParser = new SceneFileParser();

    MyScene = SceneParser->ParseSceneFromFile("Assets/Scenes/FirstSceneFile.txt","Assets/");
}
void HelloSceneFile::Tick(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 122,2,43 });
    MyScratch->SetCameraFOV(90);
    MyScratch->MeshColor = RGB{ 90,90,90,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -4.8f, -10.1f }, vec3d{ 0.0f,0.4f, 1.0f });

    for (int i = 0; i < MyScene.scene_objects.size(); i++)
    {
        int MeshId = MyScene.scene_objects[i].model_id;
        MyScratch->DrawMesh(MyScene.Meshes[MeshId], MyScene.scene_objects[i].pos, MyScene.scene_objects[i].rot, MyScene.scene_objects[i].scale);
    }
}
