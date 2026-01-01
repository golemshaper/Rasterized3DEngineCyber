
#include "Game.h"
#include "DrawScratchSpace.h"
#include "MonkeyMesh.h"
#include <cmath>
#include <SDL3/SDL_mouse.h>
#include "TextSprites.h"

void Game::Initialize()
{

    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    MyTextSprites = new TextSprites();

    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].x = sin(0.2f*i);
        bullets[i].y = cos(i)* 0.2f;
        bullets[i].z = 0;
    }
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
    RGB Smile_RGB[64] = {
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
    //MyScratch->DrawSprite(angle * 164.0f, 100 + (sin(angle * 5.0f) * 32.0f), Smile_RGB, 8, 8);

    //Real Sprite
    Sprite Smile_Sprite;
    Smile_Sprite.pixels = Smile_RGB;
    Smile_Sprite.width = 8;
    Smile_Sprite.height = 8;

    


    // Draw Cube Triangles in 3D
    //MyScratch->DrawMesh(MyScratch->MeshCube, DeltaTime);


//MOUSE USING SDL
    float mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
//CAMERA
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f}, vec3d{0.0f,1.0f, 1.0f});  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!


//MESH
    MyScratch->MeshColor = { 255,255,255,255 };
    MonkeyMesh monkeymesher; //Mesh loading tool

//--
//ENVIRONMENT
    MyScratch->DrawVerticies = sin(totalTime*8.0f)<0.0f; //Results in true or false
    //Edges
    MyScratch->DrawEdges = cos(totalTime * 8.0f) < 0.0f; //Results in true or false
    MyScratch->EdgeBrightness = (int)((abs(sin(totalTime)) + 0.5f) * 222);
    float SinMouseX = sin(mouseX * 0.01f) * 0.01f;
    float CosMouseY = cos(mouseY * 0.01f) * 0.01f;
    MyScratch->MeshColor = { 
        (int)(abs(sin(totalTime)) * 155),
        (int)(abs(cos(totalTime*2)) * 44),
        (int)(abs(sin(totalTime * 4)) * 55),
        255 
    };
    MyScratch->SetCamera(vec3d{ 0.0f, -8.0f, -3.5f }, vec3d{ cos(totalTime)*0.01f + SinMouseX,2-sin(totalTime)*0.01f + CosMouseY, 1.0f});
    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,0.0f, -4 }, vec3d{ totalTime, 0.0, 0.0, }, vec3d{ 8.0, 4.0, 4.0, });
    MyScratch->DrawEdges = false;
//NOTE: I'll want to maybe make a list of meshes to render, and sort those by z position if you draw them one by one
    MyScratch->MeshColor = { 255,255,255,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!
    MyScratch->DrawMesh(monkeymesher.GetTeapotMesh(), vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 1.12f,0.5f,2 }, vec3d{1.0, 1.0, totalTime, });
    MyScratch->DrawVerticies = false;;


//CAMERA
    MyScratch->SetCamera(vec3d{ 0.0f, -1.5f, -5.0f + sin(totalTime * 2.0f) }, vec3d{ sin(mouseX * 0.01f), cos(mouseY * 0.01f), 1.0f });


//OPTIONAL
    //Store and clear the buffer
    //MyScratch->MoveMainspaceToExtraBuffer(); MyScratch->Clear();//Clear the scren now that it's backed up 

//MESH
    //Monkey
    MyScratch->MeshColor = { 0,0,255,255 };
    MyScratch->DrawMesh(monkeymesher.GetMonkeyMesh(), vec3d{ 4.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime * 6.0f), }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))));
    //Boy
    MyScratch->MeshColor = { 255,0,0,255};
    MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,-0.0f,0.0f }, vec3d{ 1.0f, 0.0f, 0.0f, }, vec3d{ 2.0f, 2.0f, 2.0f, });

    //Athena
    MyScratch->MeshColor = { (int)abs(sin(totalTime*4.0f)*255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255};
    MyScratch->DrawMesh(monkeymesher.GetAthenaMesh(), vec3d{ -2.0f,-0.5f,-2.25f }, vec3d{ 1.0f,0.0f,3.0f }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))));
    //TEXT AT LAST MESH LOCATION
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y -8 , { 255, 255, 255, 255, }, "LV 1", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y,  {0, 255, 0, 255, }, "HP 25", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 8, { 0, 0, 255, 255, }, "MP 10", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 16, { 255, 0, 255, 255, }, "EXP 000", MyTextSprites, 1.0f );



    //  Color it
    MyScratch->MeshColor = { 255,255,255,255 };


    //3D Sprite LOC
    MyScratch->DrawSprite3D(
        Smile_Sprite, 
        vec3d{ 4.0f,0.0f,-0.25f }, 
        vec3d{ 1.0f, 0.0f, 0.0f, }, 
        vec3d{ 1.0f, 1.0f, 1.0f, }
    );
    

    //lerp/arc example
    MyScratch->DrawMesh(
        monkeymesher.GetTinyBoxMesh(),
        MyScratch->Arc(vec3d{ 2.0f,0.0f,-0.25f }, vec3d{ 0.0f,0.0f,-0.25f },1.0f,abs(sin(totalTime*2.0f))),
        vec3d{ 1.0f, 0.0f, 0.0f },
        MyScratch->Lerp(vec3d{ 1.5f,1.0f,1.5f }, vec3d{ 0.5f, 2.0f, 0.5f }, abs(sin(totalTime * 4.0f)))
    );


    //Now add the buffers for cool additive effect
    //MyScratch->AddBuffers(); //<-Remove this to only see the last thing drawn

   

    //Noise pass
    //MyScratch->MoveMainspaceToExtraBuffer();
    //MyScratch->RandomScreenFill();//Clear the scren now that contents are in the second buffer
    //MyScratch->BlendBuffers(0.12f);


    //TYPE TEXT 
    float cam_x = MyScratch->CameraTargetLoc.x;
    float cam_y = MyScratch->CameraTargetLoc.y;
    float cam_z = MyScratch->CameraTargetLoc.z;
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Look X %.2f,\nLook Y %.2f,\nLook Z %.2f", cam_x, cam_y, cam_z);   // format however you want
    MyScratch->DrawText(4, 4, { 255,255,255,255 }, buffer, MyTextSprites);

    

    typingEffect += 2*DeltaTime;
    MyScratch->DrawText(32, 32, { 255, 255, 0, 255, }, "GOODBYE \nWORLD!", MyTextSprites, typingEffect);
    if (typingEffect >= 1.5f)
    {
        //reset typing effect
        typingEffect = -1.0f;
    }

    //TEXT AT 3D LOCATION
    vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(vec3d{ 2.0f,0.0f,-0.25f });
    MyScratch->DrawText((int)textCoordinates2D.x-(10*6), (int)textCoordinates2D.y-12.0, { 0, 255, 0, 255, }, "Target!", MyTextSprites, typingEffect*2.0f);

    


    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();
    //Bullets
    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].z -= (0.02f+i)*DeltaTime;
        if (bullets[i].z < -30)
        {
            bullets[i].z = 0.0f;
        }
        MyScratch->DrawMesh(
            monkeymesher.GetAthenaMesh(),
            bullets[i],
            vec3d{ 1.0f, 0.0f, 0.0f },
            vec3d{ 0.9f,0.9f,0.9f }

        );
       

        MyScratch->MeshColor = { (int)abs(sin(totalTime + bullets[i].z * 4.0f) * 255),(int)abs(sin(totalTime + bullets[i].z * 2.0f) * 255),(int)abs(cos(totalTime + bullets[i].z * 4.0f) * 255),255 };


        //Label on bullets
        vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(bullets[i]);
        MyScratch->DrawText((int)textCoordinates2D.x, (int)textCoordinates2D.y, { 64, 128, 255, 255, }, "B", MyTextSprites, 1.0f);

    }
    MyScratch->AddBuffers();


}
