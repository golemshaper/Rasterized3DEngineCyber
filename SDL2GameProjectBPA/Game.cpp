
#include "Game.h"
#include "DrawScratchSpace.h"
#include "MonkeyMesh.h"
#include <cmath>

void Game::Initialize()
{

    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    
}
void Game::Tick(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,0,0 });

    //Draw using my function
    RGB MyColor = { 0 ,0,32 };
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
    Vertex pz = {240,160, Green};


    MyScratch->Clear(MyColor);
    MyScratch->DrawTriangle(p1, p2, p3);



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

    MyScratch->DrawTriangle(p1, p2, p3);
    MyScratch->DrawTriangle(px, p2, p1);
    
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
    MyScratch->DrawSpriteAdd(64, 64, Sprite_Smile, 8, 8);
    MyScratch->DrawSprite(angle*32.0f, 100+(sin(angle*5.0f)*32.0f), Sprite_Smile, 8, 8);


    // Draw Cube Triangles in 3D
    //MyScratch->DrawMesh(MyScratch->MeshCube, DeltaTime);

    //Store and clear the buffer
    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();//Clear the scren now that it's backed up 

    //Monkey
    MonkeyMesh monkey;
    MyScratch->fTheta += 12.0f * DeltaTime;
    MyScratch->DrawMesh(monkey.GetMonkeyMesh(), vec3d{ 1.25f,0.0f,0 }, DeltaTime);
    MyScratch->DrawMesh(monkey.GetTeapotMesh(), vec3d{ -1.12f,0.5f,0}, DeltaTime);
    //MyScratch->DrawMesh(monkey.GetTeapotMesh(), DeltaTime);
    
    //Now add the buffers for cool additive effect
 //   MyScratch->AddBuffers(); //<-Remove this to only see the last thing drawn


   

    //Noise pass
    //MyScratch->MoveMainspaceToExtraBuffer();
    //MyScratch->Random();//Clear the scren now that it's backed up 
    //MyScratch->MultiplyBuffers();

}
