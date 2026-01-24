#include "GameModelViewer.h"
#include "DrawScratchSpace.h"
#include "ModelFileParser.h"
#include "BMPReader.hpp"


void GameModelViewer::Initialize()
{
    MyScratch = new DrawScratchSpace();
	MyScratch->Initialize(RGB{0,0,0,255});
    ModelFileParser parser;
    LoadedMesh = parser.ParseFromFile("Assets/DeleteMe_Laura.txt");
    int w = 256;
    int h = 184;
    RGB* Texture = ReadBMP("Assets/DeleteMeLaura.bmp",w,h);
    MyScratch->SetTexture(Texture,w,h);
    MyScratch->TextureDrawOn = true;

}
void GameModelViewer::Tick(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,0,0,0 });
    MyScratch->SetCameraFOV(50);
    MyScratch->SetCamera(vec3d{ 0.0f, -0.0f, -1.1f }, vec3d{ 0.0f,-0.1f, 1.0f });
    MyScratch->DrawMesh(LoadedMesh, vec3d{ 0,0.3f,0 }, vec3d{ 1.5f,totalTime,0 }, vec3d{ 1,1,1 });
    
}
