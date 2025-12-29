
#include "Game.h"
#include "DrawScratchSpace.h"
#include "MonkeyMesh.h"
#include <cmath>
#include <SDL3/SDL_mouse.h>

void Game::Initialize()
{

    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    
}
void Game::Tick(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,2,8 });

    //Draw using my function
    RGB MyColor = { 0 ,0,0 };
    RGB Red = { 255,0,0 };
    RGB Green = { 0,255,0 };
    RGB Blue = {0,0,255};
    
    //MyScratch->DrawSquare(MyScratch->GetRandom(0, 240), 24, 32, MyColor);
    //MyScratch->DrawSquareMultiply(32, MyScratch->GetRandom(0, 200),32, MyColor);
    //MyScratch->DrawLine(0, 0, 200, MyScratch->GetRandom(0,200), MyColor);

    

    //Triangle
    Vertex p1 = {100,16,Blue};
    Vertex p2 = {0,32, Red * Blue };
    Vertex p3 = {200,92,Green+Blue};

    Vertex px = { 0,0, Red };
    Vertex pz = { 240,160, Green };
    Vertex pq = {88,35, Green};


 



    //Move main drawing space to the backup buffer
    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();//Clear the scren now that it's backed up 

    //Rotating Triangle
    angle += DeltaTime;
    Point pivot = { SCREEN_X / 2, SCREEN_Y / 2 };

    p1.x = MyScratch->RotatePoint({ p1.x, p1.y }, pivot, angle).x;
    p1.y = MyScratch->RotatePoint({ p1.x, p1.y }, pivot, angle).y;

    p2.x = MyScratch->RotatePoint({ p2.x, p2.y }, pivot, angle).x;
    p2.y = MyScratch->RotatePoint({ p2.x, p2.y }, pivot, angle).y;

    p3.x = MyScratch->RotatePoint({ p3.x, p3.y }, pivot, angle).x;
    p3.y = MyScratch->RotatePoint({ p3.x, p3.y }, pivot, angle).y;

    pq.x = MyScratch->RotatePoint({ pq.x, pq.y }, pivot, angle).x;
    pq.y = MyScratch->RotatePoint({ pq.x, pq.y }, pivot, angle).y;

    MyScratch->DrawTriangle(p1, p2, p3);
    MyScratch->DrawTriangle(px, p2, p1);

   // MyScratch->DrawTriangle(px, pq, pz);

    //Now add the buffers
    MyScratch->AddBuffers();

    //SPRITE
    RGB Sprite_Smile[64] = {
        // Row 0
        {0,0,0}, {0,0,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {0,0,0}, {0,0,0},
        // Row 1
        {0,0,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {0,0,0},
        // Row 2
        {255,255,0}, {255,255,0}, {1,1,1}, {255,255,0}, {255,255,0}, {1,1,1}, {255,255,0}, {255,255,0},
        // Row 3
        {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0},
        // Row 4
        {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0},
        // Row 5
        {255,255,0}, {255,255,0}, {1,1,1}, {255,255,0}, {255,255,0}, {1,1,1}, {255,255,0}, {255,255,0},
        // Row 6
        {0,0,0}, {255,255,0}, {255,255,0}, {1,1,1}, {1,1,1}, {255,255,0}, {255,255,0}, {0,0,0},
        // Row 7
        {0,0,0}, {0,0,0}, {255,255,0}, {255,255,0}, {255,255,0}, {255,255,0}, {0,0,0}, {0,0,0}
    };
    //MyScratch->DrawSpriteAdd(64, 64, Sprite_Smile, 8, 8);
    MyScratch->DrawSprite(angle * 164.0f, 100 + (sin(angle * 5.0f) * 32.0f), Sprite_Smile, 8, 8);


    // Draw Cube Triangles in 3D
    //MyScratch->DrawMesh(MyScratch->MeshCube, DeltaTime);


    //MOUSE USING SDL
    float mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    //CAMERA

    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f+ sin(totalTime*4.0f)}, vec3d{sin(mouseX * 0.01f), cos(mouseY * 0.01f), 1.0f});


    //MESH
    MonkeyMesh monkeymesher; //Mesh loading tool
    //I'll want to maybe make a list of meshes to render, and sort those by z position if you draw them one by one
    MyScratch->DrawMesh(monkeymesher.GetTeapotMesh(), vec3d{ (sinf(totalTime * 8.0f) * 0.2f) - 1.12f,0.5f,2 }, vec3d{1.0, 1.0, totalTime, });

    //Store and clear the buffer
   // MyScratch->MoveMainspaceToExtraBuffer();
   // MyScratch->Clear();//Clear the scren now that it's backed up 
    MyScratch->DrawMesh(monkeymesher.GetMonkeyMesh(), vec3d{ 2.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime*12.0f),});
    MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, 0.0f, });
   
    
    //lerp/arc example
    MyScratch->DrawMesh(monkeymesher.GetBoxMesh(), 
        MyScratch->Arc(vec3d{ 2.0f,0.0f,-0.25f }, vec3d{ 0.0f,0.0f,-0.25f },1.0f,abs(sin(totalTime*4.0f))) 
        , vec3d{1.0f, 0.0f, 0.0f,});


  //  MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,-3.0f,-0.25f }, vec3d{ 1.0f, 0.0f, 0.0f, }, 0.5f * DeltaTime);
  //  MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,.0f,-0.25f }, vec3d{ 1.0f, 0.0f, 0.0f, }, 0.5f * DeltaTime);
  
    //Now add the buffers for cool additive effect
    //MyScratch->AddBuffers(); //<-Remove this to only see the last thing drawn

   

    //Noise pass
    //MyScratch->MoveMainspaceToExtraBuffer();
    //MyScratch->Random();//Clear the scren now that it's backed up 
    //MyScratch->MultiplyBuffers();

}
