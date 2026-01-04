#pragma once
#include "GameAthenaSlashEmUp.h"
#include "MonkeyMesh.h"
#include <cmath>
#include <SDL3/SDL_mouse.h>
#include "TextSprites.h"
#include "TextFileReader.h"

void GameAthenaSlashEmUp::Initialize()
{
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    MyTextSprites = new TextSprites();

    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].x = sin(0.2f * i);
        bullets[i].y = cos(i) * 0.2f;
        bullets[i].z = 0;
    }
    player_position = { 0.0f,-0.5f,-2.25f };
}
void GameAthenaSlashEmUp::Tick(float DeltaTime)
{
    //GAME DESCRIPTION
    //This is an Auto scrolling slash-em-up action RPG (HP should be drawn on all enemies, maybe have a menu for attacking?)
    //Try to level up as much as possible before hitting the boss. Combo chains will grant more EXP
    // 
    // 
    //text timer for text box. resets when new string is sent
    textBoxProgressTick += DeltaTime;
    MyScratch->Input->Tick(DeltaTime);
    //GameModeTick(DeltaTime);
    switch (mode)
    {
    case 0:
        TitleScreenTick(DeltaTime);

        break;

    case 1:
        GameModeTick(DeltaTime);

        break;
    }
}

void GameAthenaSlashEmUp::TitleScreenTick(float DeltaTime)
{
    //SETUP
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,2,8 });
    MonkeyMesh monkeymesher; //Mesh loading tool
    //Vectors
    vec3d start_pos = vec3d{ 0,0,0 };
    vec3d end_pos = vec3d{ 0,0,-4.0f };

    working_vector = MyScratch->Lerp(working_vector,end_pos,0.5f*DeltaTime);

    //GFX
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });
    RGB end_color = { (int)abs(sin(totalTime * 4.0f) * 255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255 };
    
    working_color = MyScratch->Lerp(working_color, end_color ,0.5f * DeltaTime);

    
    MyScratch->MeshColor = working_color;
    MyScratch->DrawMesh(monkeymesher.GetAthenaMesh(), working_vector, vec3d{ 1.35f,totalTime*2.0f,0 }, vec3d{ 1,1,1 } );
    //TXT  (Center text by subtracting half the character count, and multiplying by character text width)
    MyScratch->DrawText((SCREEN_X/2)-(3*6), 32, { 255, 255, 255, 255, }, "ATHENA ", MyTextSprites, 1.0f);

    MyScratch->SetFade({0,0,0,0}, working_float);
    
    //EXIT OVER TIME
    float timeTillEnd = 5.0f;
    if (totalTime > timeTillEnd-1.0f)
    {
        working_float += 1.0*DeltaTime;
    }
    if (totalTime >= timeTillEnd)
    {
        totalTime = 0.0f;
        //EXIT SCENE
        working_float = 1.0f;
        mode = 1;
        
        //reset working float
    }

    /*TextBoxDraw("Hello world! ");*/

}

void GameAthenaSlashEmUp::GameModeTick(float DeltaTime)
{
    //MAIN GAME HERE...
    
    //Count and clear
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,2,8 });

    //CAMERA FOV
    MyScratch->SetCameraFOV(90 + (abs(cos(totalTime * 2.0f)) * 4));


    //Draw using my function
    RGB MyColor = { 0 ,0,0 };
    RGB Red = { 255,0,0 };
    RGB Green = { 0,255,0 };
    RGB Blue = { 0,0,255 };

    //MyScratch->DrawSquare(MyScratch->GetRandom(0, 240), 24, 32, MyColor);
    //MyScratch->DrawSquareMultiply(32, MyScratch->GetRandom(0, 200),32, MyColor);
    //MyScratch->DrawLine(0, 0, 200, MyScratch->GetRandom(0,200), MyColor);



    //Triangle
    Vertex p1 = { 100,16,Blue };
    Vertex p2 = { 0,32, Red * Blue };
    Vertex p3 = { 200,92,Green + Blue };

    Vertex px = { 0,0, Red };
    Vertex pz = { 240,160, Green };
    Vertex pq = { 88,35, Green };






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
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!

    //SKY FX___
    MyScratch->SetFade({ 0,0,0,0 }, { 0,0,0,0 }, { 0,64,64,255 }, { 35,0,164,255 }, sin(totalTime));
   
    LightningFX(lightning_phase, lightning);
    LightningFX(lightning_phase+3, lightning*1.5f);
    lightning += DeltaTime;
    if (lightning >= 1.5f)
    {
        lightning = 0.0f;
        lightning_phase++;
    }
    //SKY FX___



    //MESH
    MyScratch->MeshColor = { 255,255,255,255 };
    MonkeyMesh monkeymesher; //Mesh loading tool

    //--
    //ENVIRONMENT
    MyScratch->DrawVerticies = sin(totalTime * 8.0f) < 0.0f; //Results in true or false
    //Edges
    MyScratch->DrawEdges = cos(totalTime * 8.0f) < 0.0f; //Results in true or false
    MyScratch->EdgeBrightness = (int)((abs(sin(totalTime)) + 0.5f) * 222);
    float SinMouseX = sin(mouseX * 0.01f) * 0.01f;
    float CosMouseY = cos(mouseY * 0.01f) * 0.01f;
    //ground
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 155),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 44),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 55),
        255
    };
    MyScratch->SetCamera(vec3d{ 0.0f, -8.0f, -3.5f }, vec3d{ (sin(mouseX * 0.01f) * 0.1f) + cos(totalTime) * 0.01f + SinMouseX,2 - sin(totalTime) * 0.01f + CosMouseY, 1.0f });
    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,0.0f, -4 }, vec3d{ totalTime, 0.0, 0.0, }, vec3d{ 8.0, 4.0, 4.0, });
    MyScratch->DrawEdges = false;
    MyScratch->DrawVerticies = false;

    //sky
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 2),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 2),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 155),
        255
    };
    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,-5.0f, 12 }, vec3d{ -totalTime, 0.0, 0.0, }, vec3d{ 12.0, 4.0, 4.0, });
    //NOTE: I'll want to maybe make a list of meshes to render, and sort those by z position if you draw them one by one
       //teapot
    MyScratch->MeshColor = { 255,255,255,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!
    MyScratch->DrawVerticies = true;
    MyScratch->DrawMesh(monkeymesher.GetTeapotMesh(), vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 1.12f,0.5f,2 }, vec3d{ 1.0, 1.0, totalTime, });
    MyScratch->DrawVerticies = false;


    //CAMERA

    //MyScratch->SetCamera(vec3d{ 0.0f, -1.5f, -5.0f + (sin(totalTime * 2.0f) * 0.5f) }, vec3d{ sin(mouseX * 0.01f), cos(mouseY * 0.01f), 1.0f });
    vec3d camLocation = vec3d{ 0.0f, -1.5f, -5.0f + (sin(totalTime * 2.0f) * 0.5f) };
    //      YOU MUST SUBTRACT THE CAMERA LOCATION FROM THE TARGET LOCATION! ALSO, MOUSE POSITION OPTIONAL 
    vec3d camLookTarget = player_position - camLocation + vec3d{0,0,2} + vec3d{ sin(mouseX * 0.01f), cos(mouseY * 0.01f), 1.0f };
    if (last_safe_look == vec3d{ 0.0f, 0.0f, 0.0f})
    {
        last_safe_look = camLookTarget;
    }
    if (MyScratch->Distance2D(camLocation, player_position) < 0.2f)
    {
        camLookTarget = last_safe_look;

    }
    else
    {
        last_safe_look = camLookTarget;
    }
    MyScratch->SetCamera(camLocation, camLookTarget);


    
    //MESH
        //Monkey
    MyScratch->MeshColor = { 0,0,255,255 };
    MyScratch->DrawMesh(monkeymesher.GetMonkeyMesh(), vec3d{ 4.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime * 6.0f), }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))));
    //Boy
    float flash_color_select = sin(totalTime*60.0f);
    if (flash_color_select > 0.0f)
    {
        MyScratch->MeshColor = { 255,100,100,255 };
    }
    else
    {
        MyScratch->MeshColor = { 255,0,0,255 };
    }
    
    MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,-1.0f,0.0f }, vec3d{ 1.0f, 0.0f, 0.0f, } + MyScratch->LookAtRotation2D(vec3d{ 0.0f,-1.0f,0.0f },player_position*-1), vec3d{2.0f, 2.0f, 2.0f,});

    //Athena
    MyScratch->MeshColor = { (int)abs(sin(totalTime * 4.0f) * 255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255 };
   
    //player_position = player_position + MyScratch->GetMovementInput()*player_speed*DeltaTime;

    MovementUpdate(DeltaTime);

    //vec3d{ -2.0f,-0.5f,-2.25f }
    MyScratch->DrawMesh(monkeymesher.GetAthenaMesh(), 
        player_position, 
        vec3d{1.0f + (MyScratch->Input->GetMovementY()*-0.3f),0.0f,3.0f + (MyScratch->Input->GetMovementX() * 0.35f) },

        MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, 
            vec3d{ 1.2f, 0.9f, 1.2f }, 
            abs(sin(totalTime * 4.0f))));
    
    //TEXT AT LAST MESH LOCATION
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y - 8, { 255, 255, 255, 255, }, "LV 1", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y, { 0, 255, 0, 255, }, "HP 25", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 8, { 0, 0, 255, 255, }, "MP 10", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 16, { 255, 0, 255, 255, }, "EXP 000", MyTextSprites, 1.0f);



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
        MyScratch->Arc(vec3d{ 2.0f,0.0f,-0.25f }, vec3d{ 0.0f,0.0f,-0.25f }, 1.0f, abs(sin(totalTime * 2.0f))),
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



    typingEffect += 2 * DeltaTime;
    MyScratch->DrawText(32, 32, { 255, 255, 0, 255, }, "GOODBYE \nWORLD!", MyTextSprites, typingEffect);
    if (typingEffect >= 1.5f)
    {
        //reset typing effect
        typingEffect = -1.0f;
    }

    //TEXT AT 3D LOCATION
    vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(vec3d{ 2.0f,0.0f,-0.25f });
    MyScratch->DrawText((int)textCoordinates2D.x - (10 * 6), (int)textCoordinates2D.y - 12.0, { 0, 255, 0, 255, }, "Target!", MyTextSprites, typingEffect * 2.0f);




    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();
    //Bullets
    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].z -= (0.02f + i) * DeltaTime;
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

    //TEXTBOX
    
   // TextBoxDraw("You can display characters in \na box using this helper function. \nThis is part of this game, \nnot part of the engine!");
    TextBoxDraw(Reader.GetStringFromSheetTag("Intro"));
   // TextBoxDraw(Reader.GetStringFromSheetIndex(1));



    //FADE IN HERE!
    if (working_float > 0.0f) {
        //fade in
        working_float -= 0.5f * DeltaTime;
        MyScratch->SetFade({ 0,0,0,0 }, working_float);
    }

}

void GameAthenaSlashEmUp::UnhingedModeTick(float DeltaTime)
{
    //THIS IS JUST A DEMO OF THE UNHINGED RENDERING MODE BEFORE IT BECOMES A GAME. SHOW OFF NEW FEATURES HERE!
    //Count and clear
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 0,2,8 });

    //CAMERA FOV
    MyScratch->SetCameraFOV(90 + (abs(cos(totalTime * 2.0f)) * 4));


    //Draw using my function
    RGB MyColor = { 0 ,0,0 };
    RGB Red = { 255,0,0 };
    RGB Green = { 0,255,0 };
    RGB Blue = { 0,0,255 };

    //MyScratch->DrawSquare(MyScratch->GetRandom(0, 240), 24, 32, MyColor);
    //MyScratch->DrawSquareMultiply(32, MyScratch->GetRandom(0, 200),32, MyColor);
    //MyScratch->DrawLine(0, 0, 200, MyScratch->GetRandom(0,200), MyColor);



    //Triangle
    Vertex p1 = { 100,16,Blue };
    Vertex p2 = { 0,32, Red * Blue };
    Vertex p3 = { 200,92,Green + Blue };

    Vertex px = { 0,0, Red };
    Vertex pz = { 240,160, Green };
    Vertex pq = { 88,35, Green };






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
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!


    //MESH
    MyScratch->MeshColor = { 255,255,255,255 };
    MonkeyMesh monkeymesher; //Mesh loading tool

    //--
    //ENVIRONMENT
    MyScratch->DrawVerticies = sin(totalTime * 8.0f) < 0.0f; //Results in true or false
    //Edges
    MyScratch->DrawEdges = cos(totalTime * 8.0f) < 0.0f; //Results in true or false
    MyScratch->EdgeBrightness = (int)((abs(sin(totalTime)) + 0.5f) * 222);
    float SinMouseX = sin(mouseX * 0.01f) * 0.01f;
    float CosMouseY = cos(mouseY * 0.01f) * 0.01f;
    //ground
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 155),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 44),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 55),
        255
    };
    MyScratch->SetCamera(vec3d{ 0.0f, -8.0f, -3.5f }, vec3d{ (sin(mouseX * 0.01f) * 0.1f) + cos(totalTime) * 0.01f + SinMouseX,2 - sin(totalTime) * 0.01f + CosMouseY, 1.0f });
    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,0.0f, -4 }, vec3d{ totalTime, 0.0, 0.0, }, vec3d{ 8.0, 4.0, 4.0, });
    MyScratch->DrawEdges = false;
    MyScratch->DrawVerticies = false;

    //sky
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 2),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 2),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 155),
        255
    };
    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,-5.0f, 12 }, vec3d{ -totalTime, 0.0, 0.0, }, vec3d{ 12.0, 4.0, 4.0, });
    //NOTE: I'll want to maybe make a list of meshes to render, and sort those by z position if you draw them one by one
       //teapot
    MyScratch->MeshColor = { 255,255,255,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!
    MyScratch->DrawVerticies = true;
    MyScratch->DrawMesh(monkeymesher.GetTeapotMesh(), vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 1.12f,0.5f,2 }, vec3d{ 1.0, 1.0, totalTime, });
    MyScratch->DrawVerticies = false;


    //CAMERA
    MyScratch->SetCamera(vec3d{ 0.0f, -1.5f, -5.0f + (sin(totalTime * 2.0f) * 0.5f) }, player_position+vec3d{ sin(mouseX * 0.01f), cos(mouseY * 0.01f), 1.0f });

    //MESH
        //Monkey
    MyScratch->MeshColor = { 0,0,255,255 };
    MyScratch->DrawMesh(monkeymesher.GetMonkeyMesh(), vec3d{ 4.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime * 6.0f), }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))));
    //Boy
    MyScratch->MeshColor = { 255,0,0,255 };
    MyScratch->DrawMesh(monkeymesher.GetBoyMesh(), vec3d{ 0.0f,-0.0f,0.0f }, vec3d{ 1.0f, 0.0f, 0.0f, }, vec3d{ 2.0f, 2.0f, 2.0f, });

    //Athena
    MyScratch->MeshColor = { (int)abs(sin(totalTime * 4.0f) * 255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255 };
    MyScratch->DrawMesh(monkeymesher.GetAthenaMesh(), vec3d{ -2.0f,-0.5f,-2.25f }, vec3d{ 1.0f,0.0f,3.0f }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))));
    //TEXT AT LAST MESH LOCATION
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y - 8, { 255, 255, 255, 255, }, "LV 1", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y, { 0, 255, 0, 255, }, "HP 25", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 8, { 0, 0, 255, 255, }, "MP 10", MyTextSprites, 1.0f);
    MyScratch->DrawText((int)MyScratch->Get2DPointFromLastLocation().x - 12, (int)MyScratch->Get2DPointFromLastLocation().y + 16, { 255, 0, 255, 255, }, "EXP 000", MyTextSprites, 1.0f);



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
        MyScratch->Arc(vec3d{ 2.0f,0.0f,-0.25f }, vec3d{ 0.0f,0.0f,-0.25f }, 1.0f, abs(sin(totalTime * 2.0f))),
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



    typingEffect += 2 * DeltaTime;
    MyScratch->DrawText(32, 32, { 255, 255, 0, 255, }, "GOODBYE \nWORLD!", MyTextSprites, typingEffect);
    if (typingEffect >= 1.5f)
    {
        //reset typing effect
        typingEffect = -1.0f;
    }

    //TEXT AT 3D LOCATION
    vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(vec3d{ 2.0f,0.0f,-0.25f });
    MyScratch->DrawText((int)textCoordinates2D.x - (10 * 6), (int)textCoordinates2D.y - 12.0, { 0, 255, 0, 255, }, "Target!", MyTextSprites, typingEffect * 2.0f);




    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();
    //Bullets
    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].z -= (0.02f + i) * DeltaTime;
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

    //TEXTBOX
    TextBoxDraw("You can display characters in \na box using this helper function. \nThis is part of this game, \nnot part of the engine!");


}

void GameAthenaSlashEmUp::TextBoxDraw(const char* input)
{
    if (input != previous_text)
    {
        textBoxProgressTick = 0.0f;
        previous_text = input;
    }

    //Debug
    //char buffer[64];
    //snprintf(buffer, sizeof(buffer), "Index %.2f", (float)Reader.CurrentLine);
    //MyScratch->DrawText(12, 132, { 0, 255, 255, 255, }, buffer, MyTextSprites, 1.0f);
    //MyScratch->DrawText(12,152, { 0, 255, 255, 255, }, Reader.StoredRowData[Reader.CurrentLine].Event.c_str(), MyTextSprites, 1.0f);
    



    if (textBoxProgressTick > 5.0f)
    {
        //Auto-Advanced text until "End" tag found
        if (Reader.HasEventAtIndex(Reader.CurrentLine,"End")==false && Reader.HasEventAtIndex(Reader.CurrentLine + 1, "Start") == false)
        {
            textBoxProgressTick = 0.0f;
            TextBoxDraw(Reader.GetStringFromSheetIndex(Reader.CurrentLine+1));
        }
        
        return;
    }


    //TEXT BOX
    int boarder = 2;
    //Fill
    MyScratch->DrawRectangle(
        boarder, SCREEN_Y - (64 + boarder), 
        SCREEN_X - (boarder * 2), 
        64, 
        RGB{ 0,0,255,128 },
        RGB{ 0,222,0,128 },
        RGB{ 0,0,255,0 },
        RGB{ 0,0,111,0 }

    );

    //Top Line
    MyScratch->DrawLine(boarder, SCREEN_Y - (64 + boarder), SCREEN_X - boarder-1, SCREEN_Y - (64 + boarder), RGB{ 222,222,222,255 });
    //Bottom Line
    MyScratch->DrawLine(boarder, SCREEN_Y - boarder, SCREEN_X - boarder - 1, SCREEN_Y -boarder, RGB{ 222,222,222,255 });
    //Right Side
    MyScratch->DrawLine(SCREEN_X - boarder - 1, SCREEN_Y - (64 + boarder), SCREEN_X - boarder - 1, SCREEN_Y - boarder, RGB{ 222,222,222,255 });
    //Left Side
    MyScratch->DrawLine(boarder , SCREEN_Y - (64 + boarder), boarder , SCREEN_Y - boarder, RGB{ 222,222,222,255 });

    //Text
    MyScratch->DrawText(boarder + 2, SCREEN_Y - (64), { 255, 255, 255, 255, }, input, MyTextSprites, textBoxProgressTick * 0.8f);

    //Blinking cursor 
    if (sin(totalTime * 8.0f) > 0.0f)
    {
        
        MyScratch->DrawText(SCREEN_X - 10, SCREEN_Y - boarder - 7, { 0, 255, 255, 255, }, "|", MyTextSprites, 1.0f);

    }
}

void GameAthenaSlashEmUp::LightningFX(int phase, float progress)
{
    const int total_verts = 23;
    const int total_rand = 16;
    const int pseudo_random[16] =
    {
        1, -1, 4, -9, 12, -19, 17, -5,1, -5, 9, -3, 3, -1, 4, -15
    };

    int offset = (phase % total_verts) * 3;
    vec3d start = { SCREEN_X * 0.5f + offset, 0, 0 };
    vec3d end = { SCREEN_X * 0.25f + offset+2, SCREEN_Y, 0 };
    vec3d prev = start;

    float additional_progress_time = 0.0f;
   

    RGB StartColor = { 255,0,255,(int)(255*(1-progress))};
    RGB EndColor = {255,255,0,0};

    
    for (int i = 0; i < total_verts; i++)
    {
        float t0 = (float)(i + 1) / (float)total_verts;

        // Step toward the end point
        vec3d halfway = MyScratch->Lerp(start, end, t0);

        // Apply horizontal offset
        halfway.x += pseudo_random[(int)(i+ phase)% total_rand];

        // Draw segment
        MyScratch->DrawLine(prev.x, prev.y, halfway.x, halfway.y,MyScratch->Lerp(StartColor, EndColor, t0*2.0f));

        prev = halfway;
    }
}

void GameAthenaSlashEmUp::MovementUpdate(float DeltaTime)
{
    vec3d forward = MyScratch->Normalize(MyScratch->CameraTargetLoc); //is this always correct if the camera moves? No idea...But I suspect so...

    vec3d up = { 0, 1, 0 };
    vec3d right = MyScratch->Normalize(MyScratch->CrossProduct(up, forward));

    //Flatten ground movement
    forward.y = 0;
    right.y = 0;

    forward = MyScratch->Normalize(forward);
    right = MyScratch->Normalize(right);

    //Apply Movement
    vec3d move = (right * MyScratch->Input->GetMovementX()) + (forward * MyScratch->Input->GetMovementY());
    player_position = player_position + move *player_speed* DeltaTime;


}