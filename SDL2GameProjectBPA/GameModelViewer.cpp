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
    LoadedMesh2 = parser.ParseFromFile("Assets/KartaModels.txt");
    //Palette
    int bpw = 64;
    int bph = 64;
    BasicPalette = ReadBMP("Assets/BasicPalette.bmp", bpw, bph);
    //Laura
    int w = 256;
    int h = 184;
    LauraTexture = ReadBMP("Assets/DeleteMeLaura.bmp", w, h);
    //DefaultTexture
    MyScratch->SetTexture(LauraTexture,w,h);
    MyScratch->TextureDrawOn = true;

}
void GameModelViewer::Tick(float DeltaTime)
{
    MyScratch->Clear(RGB{ 0,0,0,0 });

    //Other model:
   // DrawModelUsingBasicPalette(DeltaTime);
    //Laura
    DrawLaura(DeltaTime);

    
}

void GameModelViewer::DrawLaura(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->SetCameraFOV(30);
    MyScratch->MeshColor = RGB{ 90,90,90,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -0.8f, -3.1f }, vec3d{ 0.0f,0.20f, 1.0f });
    MyScratch->EdgeBrightness = 0.5f;
    MyScratch->SetTexture(LauraTexture, 256, 184);
    MyScratch->DrawMesh(LoadedMesh, vec3d{ 0,0.0f,0 }, vec3d{ 1.55f,totalTime,0 }, vec3d{ 1,1,1 }, true);
}

void GameModelViewer::DrawModelUsingBasicPalette(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->SetCameraFOV(30);
    //MyScratch->MeshColor = RGB{ 190,190,190,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -0.0f, -1.1f }, vec3d{ 0.0f,0.02f, 1.0f });
    MyScratch->EdgeBrightness = 0.5f;
    MyScratch->DrawUnlit = true;
    MyScratch->SetTexture(BasicPalette, 64, 64);
    MyScratch->DrawMesh(LoadedMesh2, vec3d{ 0,0.0f,0 }, vec3d{ 1.5f,totalTime,totalTime }, vec3d{ 0.15f,0.15f,0.15f }, true);
    //darken image a bit
    MyScratch->DrawSquare(0, 0, 512, RGB{ 0,0,0,164 });
}
