#pragma once
#include "GameAthenaSlashEmUp.h"
#include "MonkeyMesh.h"
#include <cmath> 
#include <SDL3/SDL_mouse.h>
#include "TextSprites.h"
#include "TextFileReader.h"
#include "ModelFileParser.h"
#include "BMPReader.hpp"

void GameAthenaSlashEmUp::Initialize()
{
    //ENGINE
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    MyScratch->CopyBufferToBuffer(MyScratch->MainSpace, BlurBuffer);
    MyTextSprites = new TextSprites();
    Reader = new TextFileReader();
    Reader->ReadText();
    ModelFileParser parser;
    //STATES (on stack)
    sm.MapState(FirstStateId, [this] {StateMachineHelloWorldTick(); });
   
    //PLAYER STATES
    sm.MapState(State_AthenaIdle, [this] {PlayerIdleState(); });
    sm.MapState(State_AthenaAttack, [this] {PlayerAttackState(); });
    sm.SetState(State_AthenaIdle);



    //athena body bullets
    for (int i = 0; i < bullet_count; ++i)
    {
        bullets[i].x = sin(0.2f * i);
        bullets[i].y = cos(i) * 0.2f;
        bullets[i].z = 0;
    }
    player_position = { -2.0f,-0.5f,-2.25f };

    //bullet arc shots
    for (int i = 0; i < bullet_arcshots_count; ++i)
    {
        bullet_arcshots[i].x = sin(0.2f * i);
        bullet_arcshots[i].y = cos(i) * 0.2f;
        bullet_arcshots[i].z = 0;
    }
    MonkeyMesh monkeyMesh;
    //CREATE PLAYER (Implied Actor Struct!)
    PlayerID = CreateActor({ 
        monkeyMesh.GetTeapotMesh(), //Make Athena
        vec3d{ -2.0f,-0.5f,-2.25f},
        vec3d{1.0f,0.0f,0.0f},
        vec3d{0.5f,0.5f,0.5f},
        RGB{255,255,255,255},
        4.0f,
        0.0f
        });
    //Athena animated
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F0.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F1.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F1.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F2.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F3.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F4.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F5.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F6.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_F0.txt"));
    PlayerMeshSequence.push_back(parser.ParseFromFile("Assets/Athena_Neutral.txt"));
    CurrentAthenaFrame = parser.ParseFromFile("Assets/Athena_Neutral.txt");
    AlienMesh = parser.ParseFromFile("Assets/AlienModel.txt");


    ////Load textures like this:
    //int w2 = 128; int h2 = 112;
    //RGB* Image02 = ReadBMP("Assets/011.bmp", w2, h2);
    //MyScratch->SetTexture(Image02, w2, h2);


    //Setup Game States...
}
void GameAthenaSlashEmUp::Tick(float DeltaTime)
{
    //Modify the speed of the game by changing GameSpeed
    DeltaTime = DeltaTime * GameSpeed;
    //GAME DESCRIPTION
    // 
    // 
    // Ideas:
    // https://nexusrexdev.github.io/gamedevcorner/analysis/2023/07/19/enemy-design-shmups.html
    // https://www.gamedeveloper.com/design/build-a-bad-guy-workshop---designing-enemies-for-retro-games
    // 
    // 
    //This is an Auto scrolling slash-em-up action RPG (HP should be drawn on all enemies, maybe have a menu for attacking?)
    //Try to level up as much as possible before hitting the boss. Combo chains will grant more EXP
    // 
    // 
    // Fly in to shop keepers to trigger a shopping sequence to buy upgrades for your character
    // 
    // try to get strong enough to overcome the boss
    // 
    // 
    // consider some enemy types that trigger a turn based combat encounter!
    // 
    // 
    // 
    //text timer for text box. resets when new string is sent
    
    MyScratch->Input->Tick(DeltaTime);
    
    //TODO REPLACE THIS WITH THE GAME-STATE MACHINE (sm)
    switch (mode)
    {
    case 0:

       /* if (demoWait > 0.0f)
        {
            demoWait -= DeltaTime;
            return;
        }*/
        TitleScreenTick(DeltaTime);

        break;

    case 1:
        GameModeTick(DeltaTime);

        break;
    }
    sm.Tick(DeltaTime);

}

void GameAthenaSlashEmUp::TitleScreenTick(float DeltaTime)
{
    //TITLE SCREEN
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
    MyScratch->DrawText((SCREEN_X/2)-(3*12), 32, { 255, 255, 255, 255, }, "CYBER-ATHENA ", MyTextSprites, 1.0f);

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
    AccumulatedBlur(0.70f);
}

void GameAthenaSlashEmUp::GameModeTick(float DeltaTime)
{
    //GAME MODE~
    //----------------------
    //MOUSE USING SDL
    //----------------------
    float mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);
    //Mesh loading tool
    MonkeyMesh monkeymesher;
    //SPRITE (PLEASE MOVE THIS TO SOME KIND OF SPRITE HEADER)
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

    //Real Sprite
    Sprite Smile_Sprite = { Smile_RGB, 8,8 };
    //----------------------
    //MAIN GAME HERE........
    //----------------------

    //Count total time
    totalTime += DeltaTime;
    //Canvas
    MyScratch->Clear(RGB{ 0,2,8 });
    MyScratch->ClearZBufffer();
    MyScratch->ZWriteOn = true;
    if (drawBuffer == 7)
    {
        MyScratch->ZWriteOn = false; //show without depth buffer. Show off 2D z sort
    }
    //Target List reset
    ClearTargetableLocAry(TargetableLocations,30); //used for targeting enemies
    
    /* MyScratch->DrawSpriteAdd(64, 64, Smile_RGB, 8, 8);
    MyScratch->DrawSprite(angle * 164.0f, 100 + (sin(angle * 5.0f) * 32.0f), Smile_RGB, 8, 8);*/

    //BKG Draw
    DrawWorldOneBKG(DeltaTime,mouseX,mouseY);


    //----------------------
    // CAMERA MAIN CAM
    //----------------------
    camLocation = vec3d{ 0.0f, -1.5f, -5.0f + (sin(totalTime * 2.0f) * 0.5f) };
    psudoCamLocation = vec3d{ 0.0f, -1.5f, -5.0f };
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

    //----------------------
    //MESH
    //---------------------- 
    
    //Monkey
    MyScratch->MeshColor = { 0,0,255,255 };
    MyScratch->DifferDrawMesh(MyScratch->WaveMesh(monkeymesher.GetMonkeyMesh(), 12 * totalTime, 0.2f), vec3d{ 4.0f,0.0f,-0.25f }, vec3d{ 1.0f, 0.0f, sin(totalTime * 6.0f), }, MyScratch->Lerp(vec3d{ 0.9f,1.2f,0.9f }, vec3d{ 1.2f, 0.9f, 1.2f }, abs(sin(totalTime * 4.0f))),true);
    AddTargetableLOC(vec3d{ 4.0f,0.0f,-0.25f });

    //Alien (Boy)
    float flash_color_select = sin(totalTime*60.0f);
    if (flash_color_select > 0.0f)
    {
        MyScratch->MeshColor = { 255,160,160,255 };
    }
    else
    {
        MyScratch->MeshColor = { 255,0,0,255 };
    }
    MyScratch->DifferDrawMesh(AlienMesh, vec3d{ 0.0f,-1.0f,0.0f }, vec3d{ 1.75f, 0.0f, 0.0f, } + MyScratch->LookAtRotation2D(vec3d{ 0.0f,-1.0f,0.0f }, (player_position * -1) + vec3d{0,0,10}), vec3d{ 2.0f, 2.0f, 2.0f, }, true);
    AddTargetableLOC(vec3d{ 0.0f,-1.0f,0.0f });

    //DRAW ENEMIES HERE:
    // 
    // 
    // 
    // USE DrawActor() to draw an actor and make it targetable automatically.
    // Create a function that makes enemies of X type and adds them as actors, and sets their locations where they should spawn in from
    // Make a function that moves that cluster of actors (inside of their own list mind!) on to their starting locations from off screen (Arc position)
    // 
    // Tick the enemy and have them attempt to leave the screen. Consider giving each actor a state machine! Either have a state machine pool assigned to their ID like a component system,
    // or just stick it in a class/struct
    // 
    // 
    // 
    // 
    // 
    // 
    // 
    // 
    //Athena
    MyScratch->MeshColor = { (int)abs(sin(totalTime * 4.0f) * 255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255 };
    MovementUpdate(DeltaTime);



//NOTE 1: DifferDrawMesh is a drop-in replacement for the regular DrawMesh function. It'll only draw when DrawSortedDifferedMeshes() is called
//NOTE 2: use regular DrawMesh() along with  MyScratch->ZOffsetFloat = 10;  Besure to turn the offset off when you are done
  
    MyScratch->DifferDrawMesh(CurrentAthenaFrame,
        player_position,
        //old rotation was x=1.0
        vec3d{ 1.0f + (MyScratch->Input->GetMovementY() * -0.3f),0.0f,3.0f + (MyScratch->Input->GetMovementX() * 0.35f) },
        MyScratch->Lerp(vec3d{ 0.9f * temp_player_flip,1.2f,0.9f },
            vec3d{ 1.2f * temp_player_flip , 0.9f, 1.2f },
            abs(sin(totalTime * 4.0f))),
        true
    );

    //When we replace the player with the PlayerID, we will color it like this:
    AllActors[PlayerID].meshColor = { (int)abs(sin(totalTime * 4.0f) * 255),(int)abs(sin(totalTime * 2.0f) * 255),(int)abs(cos(totalTime * 4.0f) * 255),255 };

   // 
   // 
//DRAW ANOTHER ATHENA, BUT USING THE ADD ACTOR VERSION!
    //REMOVE THIS TO MAKE IT MOVE WITH THE PLAYER!
    AllActors[PlayerID].loc.x = sin(totalTime * 2.0f) - 4.0f;
    AllActors[PlayerID].loc.y = cos(totalTime * 2.0f);
    AllActors[PlayerID].loc.z = -3;
    DrawActorsFromList(AllActors); //IN REALITY WE WOULD NOT DO ALL ACTORS AT ONCE! WE'D DO CATEGORIES OF ACTORS! EACH AND WE'D HAVE FUNCTIONS TO DRAW AND MOVE THEM AT SAME TIME!
//DRAW ANOTHER ATHENA, BUT USING THE ADD ACTOR VERSION!

 

    //DRAW PSUDO Z SORTED MESHES (Or Real Sorted if ZWriteOn)
    //Set ZWriteOn to false to get old 2D sorting feature! :: MyScratch->ZWriteOn = false;
    MyScratch->DrawSortedDifferedMeshes();


    //------------------------------
    // Bullets
    //------------------------------

    //TODO: Store closest locations, but only update the list if not shooting! Once you shoot, you 
    //should be locked on, and no longer auto-target
    TickArcShots(player_position, GetClosestVectorFromList(player_position, TargetableLocations, 30), DeltaTime);
    MyScratch->Draw3DLine(player_position, GetClosestVectorFromList(player_position, TargetableLocations, 30), RGB_Yellow);
    /*
    * TODO/IDEA:
    * a scene exporter for blender
    * could create 2 vectors per object
    * 
    * For a mesh, it would need to export a name for the Mesh, based off of the object name, and LOC and ROT values
    * A camera would be the same, but have no mesh
    * 
    * You could call a function on init to create actors and cameras at those locations, and loop through and draw them!
    * 
    * 
    * 
    */

    //Reset Draw color to white.
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


   
    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();
    //Bullets (Athena ground add FX
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




//TOGGLE VIEWS
    if (MyScratch->Input->GetToggleDepthKey())
    {
        sm.SetState(State_AthenaAttack);
        MyScratch->Input->ResetToggleDepthKey();
        drawBuffer++;
        if (drawBuffer > 7)drawBuffer = 0;
    }
    switch (drawBuffer)
    {
    case 0:
        MyScratch->TextureDrawOn = false;
        break; 
    case 1:
        //Z
        MyScratch->DrawZBufffer();
        break;
    case 2:
        MyScratch->PushBackDepthBuffer(-240); //fix pushback terrain so it's visible in render
        MyScratch->DrawZBufffer();
        MyScratch->BrightnessContrastOnBuffer(MyScratch->MainSpace, -0.2f, 1.01f);
        break;
    case 3:
        ////FOG FX BY DEPTH
        MyScratch->PushBackDepthBuffer(-240); //fix pushback terrain so it's visible in fog
        MyScratch->MoveMainspaceToExtraBuffer();
        MyScratch->DrawZBuffferMaskSky();
        MyScratch->MultiplyBuffers();
        break;
    case 4:
        //Noise pass
        MyScratch->MoveMainspaceToExtraBuffer();
        MyScratch->RandomScreenFill();//Clear the scren now that contents are in the second buffer
        MyScratch->BlendBuffers(0.12f);
        break;
    case 5:
        AccumulatedBlur(0.5f);
        
        break;

    case 6:
        //Textures enabled
        MyScratch->TextureDrawOn = true;
        break;
    case 7:
        MyScratch->TextureDrawOn = false;
        //Textures enabled
        MyScratch->ZWriteOn = false;//WE WILL CHECK THE DRAW STATE, AND TURN OFF AT TOP OF THIS FUNCTION!
        break;
    }
   
    DrawHUD(DeltaTime);
}

void GameAthenaSlashEmUp::DrawWorldOneBKG(float DeltaTime, float mouseX, float mouseY)
{
    //MyScratch->SetCameraFOV(90 + (abs(cos(totalTime * 2.0f)) * 4));
    MonkeyMesh monkeymesher; //Mesh loading tool
    //BKG FX
    NewWaveFXDraw(DeltaTime);
    //CAMERA FOV
    
    //----------------------
    //CAMERA
    //----------------------
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!

    //SKY FX___
    MyScratch->SetFade({ 0,0,0,0 }, { 0,0,0,0 }, { 0,64,64,255 }, { 35,0,164,255 }, sin(totalTime));

    LightningFX(lightning_phase, lightning);
    LightningFX(lightning_phase + 3, lightning * 1.5f);
    lightning += DeltaTime;
    if (lightning >= 2.5f)
    {
        lightning = 0.0f;
        lightning_phase++;
    }
    //SKY FX___


    //----------------------
    //MESH
    //----------------------
    MyScratch->MeshColor = { 255,255,255,255 };

    //----------------------
    //ENVIRONMENT
    //----------------------
    MyScratch->DrawVerticies = sin(totalTime * 8.0f) < 0.0f; //Results in true or false
    //----------------------
    //Edges
    //----------------------
    MyScratch->DrawEdges = cos(totalTime * 8.0f) < 0.0f; //Results in true or false
    MyScratch->EdgeBrightness = (int)((abs(sin(totalTime)) + 0.5f) * 222);
    float SinMouseX = sin(mouseX * 0.01f) * 0.01f;
    float CosMouseY = cos(mouseY * 0.01f) * 0.01f;
    //----------------------
    //Ground
    //----------------------
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 155),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 44),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 55),
        255
    };
    MyScratch->SetCamera(vec3d{ 0.0f, -8.0f, -3.5f }, vec3d{ (sin(mouseX * 0.01f) * 0.1f) + cos(totalTime) * 0.01f + SinMouseX,2 - sin(totalTime) * 0.01f + CosMouseY, 1.0f });

    //----------------------
    //Terrain
    //----------------------

    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ player_position.x * -0.5f,0.0f, -4 }, vec3d{ totalTime, 0.0, 0.0, }, vec3d{ 15.0, 4.0, 4.0, });
    MyScratch->DrawEdges = false;
    MyScratch->DrawVerticies = false;
    //----------------------
    //Sky
    //----------------------
    MyScratch->MeshColor = {
        (int)((abs(sin(totalTime)) + 0.5f) * 2),
        (int)((abs(cos(totalTime * 2)) + 0.5f) * 2),
        (int)((abs(sin(totalTime * 4)) + 0.5f) * 155),
        255
    };

    MyScratch->DrawMesh(monkeymesher.GetTerrainBall(), vec3d{ 0.0f,-5.0f, 12 }, vec3d{ -totalTime, 0.0, 0.0, }, vec3d{ 12.0, 4.0, 4.0, });
    //teapot
    MyScratch->MeshColor = { 255,255,255,255 };
    MyScratch->SetCamera(vec3d{ 0.0f, -1.0f, -4.0f }, vec3d{ 0.0f,1.0f, 1.0f });  //By calling multiple SetCamera calls during drawing, you can make things like a skybox, that don't move, but follow the rest of the worlds rotation!
    MyScratch->DrawVerticies = true;
    MyScratch->DrawMesh(monkeymesher.GetTeapotMesh(), vec3d{ (sinf(totalTime * 4.0f) * 0.2f) - 1.12f,0.5f,2 }, vec3d{ 1.0, 1.0, totalTime, }, vec3d{ 1,1,1 });
    MyScratch->DrawVerticies = false;
    //morphing cube
     //MORPH TEST:
    /*Mesh morph = MyScratch->MorphMesh(monkeymesher.GetPyrimidBoxMorph0(), monkeymesher.GetPyrimidBoxMorph(), abs(sin(totalTime)));
    MyScratch->DrawMesh(morph, vec3d{ 2.0f, 0.0f,0.0f }, vec3d{ 1.5f,0,0 }, vec3d{ 0.25f,0.25f,0.25f });*/

    MyScratch->PushBackDepthBuffer(240); //Push back so we don't clip the actors
}

void GameAthenaSlashEmUp::ReadNewText(const char* input)
{
    previous_text = "";
    RequestedText = input;
    
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
        if (Reader->HasEventAtIndex(Reader->CurrentLine,"End")==false && Reader->HasEventAtIndex(Reader->CurrentLine + 1, "Start") == false)
        {
            textBoxProgressTick = 0.0f;
            TextBoxDraw(Reader->GetStringFromSheetIndex(Reader->CurrentLine+1));
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

    //Coordinates
    const int textX = boarder + 2;
    const int textY = SCREEN_Y - (64);
    //Shadow
    MyScratch->DrawText(textX+1, textY+1, { 1, 1, 1, 255, }, input, MyTextSprites, textBoxProgressTick * 0.8f);
    //Text
    MyScratch->DrawText(textX, textY, { 255, 255, 255, 255, }, input, MyTextSprites, textBoxProgressTick * 0.8f);

    //Blinking cursor 
    if (sin(totalTime * 8.0f) > 0.0f)
    {
        
        MyScratch->DrawText(SCREEN_X - 10, SCREEN_Y - boarder - 7, { 0, 255, 255, 255, }, "|", MyTextSprites, 1.0f);

    }
}

void GameAthenaSlashEmUp::LightningFX(int phase, float progress)
{
    if (progress <= 0.02f)
    {
        MyScratch->DrawRectangle(0, 0, SCREEN_X, (int)SCREEN_Y*0.75f, RGB{ 255,255,0,64 }, RGB{ 255,255,255,64 }, RGB{ 255,255,255,0 }, RGB{ 255,255,0,0 });
    }
    const int total_verts = 23;
    //const int total_rand = 16;
    /*const int pseudo_random[16] =
    {
        1, -1, 4, -9, 12, -19, 17, -5,1, -5, 9, -3, 3, -1, 4, -15
    };*/

    int offset = (phase % total_verts) * 3;
    vec3d start = { SCREEN_X * 0.5f + offset, 0, 0 };
    vec3d end = { SCREEN_X * 0.25f + offset+2, SCREEN_Y, 0 };
    vec3d prev = start;

    float additional_progress_time = 0.0f;
   
    int mult = 2; //make color brighter
    RGB StartColor = { 255* mult,0,255* mult,(int)(255*(1-progress))};
    RGB EndColor = {255* mult,255* mult,0,0};

    
    for (int i = 0; i < total_verts; i++)
    {
        float t0 = (float)(i + 1) / (float)total_verts;

        // Step toward the end point
        vec3d halfway = MyScratch->Lerp(start, end, t0);

        // Apply horizontal offset
        //halfway.x += pseudo_random[(int)(i + phase) % total_rand];
        halfway.x += MyScratch->GetNext(-8,8);

        // Draw segment
        MyScratch->DrawLine(prev.x, prev.y, halfway.x, halfway.y,MyScratch->Lerp(StartColor, EndColor, t0*2.0f));

        prev = halfway;
    }
}

void GameAthenaSlashEmUp::NewWaveFXDraw(float DeltaTime)
{

    //Draw using my function1
    RGB MyColor = { 0 ,0,0 };
    RGB Red = { 255,0,0 };
    RGB Green = { 0,255,0 };
    RGB Blue = { 0,0,255 };

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


    //TRY A RANDOM FILL OVER BACKGROUND ON AND OFF. IT LOOKS RAD WHEN THE LIGHTNING STRIKES: MyScratch->RandomScreenFill();


       //Now add the buffers
    MyScratch->AddBuffers();
}

void GameAthenaSlashEmUp::MovementUpdate(float DeltaTime)
{


    vec3d last_safe_pos = player_position;
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
    AllActors[PlayerID].loc = AllActors[PlayerID].loc + move * AllActors[PlayerID].speed * DeltaTime;
   // AddTargetableLOC(AllActors[PlayerID].loc); //WARNING, WHEN WE SITCH TO PLAYER ID, THIS WILL ALWAYS BE ON PLAYER!!! (Right now it's the teapot)



    //Apply rotation
    AllActors[PlayerID].rot = vec3d{ 1.0f + (MyScratch->Input->GetMovementY() * -0.3f),0.0f,3.0f + (MyScratch->Input->GetMovementX() * 0.35f) };

    //CAMERA COLLIDER:
    if (MyScratch->SquaredDistance2D(player_position, psudoCamLocation) < 0.11f)
    {
        player_position = last_safe_pos;
    }
    if (MyScratch->SquaredDistance2D(player_position, psudoCamLocation) > 140.0f)
    {
        player_position = last_safe_pos;
    }

}

void GameAthenaSlashEmUp::TickArcShots(vec3d start, vec3d end, float DeltaTime)
{
    MyScratch->ZOffset = 25; //draw on top to make it better

    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear();

    progress_for_arc_shots += 1.0f*DeltaTime;
    
    MonkeyMesh monkeymesher;
    int shotType = 0;
    //bullet_arcshots_count
    if (MyScratch->Input->GetFireOneHold())
    {
        Arc_shot_Max_bullet_loc_Fired++;
        if (Arc_shot_Max_bullet_loc_Fired > bullet_arcshots_count)
        {
            Arc_shot_Max_bullet_loc_Fired = bullet_arcshots_count;
        }
    }
    else
    {
        if (Arc_shot_Max_bullet_loc_Fired >= 1)
        {
            Arc_shot_Max_bullet_loc_Fired--;
        }
        if (Arc_shot_Max_bullet_loc_Fired < 0)
        {
            progress_for_arc_shots = 0;
            Arc_shot_Max_bullet_loc_Fired = 0;
        }
    }
    
    for (int i = 0; i< Arc_shot_Max_bullet_loc_Fired; ++i)
    {
     
       
        

        MyScratch->MeshColor = { (int)abs(sin(totalTime + bullet_arcshots[i].z * 4.0f) * 355),(int)abs(sin(totalTime + bullet_arcshots[i].z * 2.0f) * 355),(int)abs(cos(totalTime + bullet_arcshots[i].z * 4.0f) * 355),255 };
       // MyScratch->MeshColor = { 255,0,255 };
        float offset_time_progress = (progress_for_arc_shots + (i*0.1f));
        float wrapped = fmod(offset_time_progress, 1.0f);
        //end calc
        if (wrapped <= 0.1f)
        {
            bullet_arcshots_end_target[i] = end;
        }



       




        
        vec3d current_end = bullet_arcshots_end_target[i];


        vec3d offset = { i * 0.05f,i * 0.05f,i * 0.05f };

        float direction = 1.0f;
        direction = sin(offset_time_progress * 2.0f);
        

        

        switch (shotType)
        {
            case 0:
                bullet_arcshots[i] = MyScratch->ArcHorizontal(start+ offset, current_end, 2.0f, wrapped, direction);
                break;
            case 1:
                bullet_arcshots[i] = MyScratch->ArcHorizontal(start+ offset, current_end, 2.0f, wrapped, direction);
                break;
            case 2: 
                bullet_arcshots[i] = MyScratch->Arc(start+ offset, current_end, 2.0f, wrapped, direction);
                break;
            case 3:
                bullet_arcshots[i] = MyScratch->Arc(start+ offset, current_end, 2.0f, wrapped, direction);
                break;

        }
        //calculate both arc vectors, and lerp between them for angle arcs
       
        //collision not needed since we know it collides at progress ==1 at target location... bool collision_result = IsColliding(bullet_arcshots[i], end, 0.5f);

        
        
        
    //DRAW BULLET::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        MyScratch->DrawMesh(
            monkeymesher.GetBoxMesh(),
            bullet_arcshots[i],
            vec3d{ 1.0f, 0.0f, 0.0f },
            vec3d{ 0.2f,0.2f,0.2f }

        );


      //DONT LIKE IT:  AccumulatedBlur(0.25f); //add motion blur only to shots!

        //Alphabet
        shotType++;
        if (shotType >= 4)shotType = 0;
        //Label on bullets
        vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(bullet_arcshots[i]);
        const char* AlphabetSoup = "LuckShotLuckShot";
        char letter[2] = { AlphabetSoup[i%17], '\0' };

        MyScratch->DrawText((int)textCoordinates2D.x, (int)textCoordinates2D.y, { 155 + i, 155 + i, 155 + i, 255, }, letter, MyTextSprites, 1.0f);

    }
   
    MyScratch->AddBuffers();
    MyScratch->ZOffset = 0;
   
}

void GameAthenaSlashEmUp::CircleTransition(float reveal)
{
    MyScratch->MoveMainspaceToExtraBuffer();
    MyScratch->Clear(RGB{ 0,0,0,0 });
    MyScratch->DrawFilledCircle(SCREEN_X / 2, SCREEN_Y / 2, (int)((SCREEN_X*1.5f )* reveal*0.5f) , RGB{ 255,255,255,255 });
    MyScratch->ApplyMask();
    //extra color around circle cutout
    MyScratch->DrawCircle(SCREEN_X / 2, SCREEN_Y / 2, (int)((SCREEN_X * 1.5f) * reveal * 0.5f), RGB{ 23,255,166,255 });


}

void GameAthenaSlashEmUp::DrawReticle(int x, int y, int radius, float progress)
{
    const int orbitCount = 4;

    for (int i = 0; i < orbitCount; ++i)
    {
        float angle = progress + (i * (MyScratch->PI / 2.0f)); 

        int ox = x + static_cast<int>(SDL_cosf(angle) * radius);
        int oy = y + static_cast<int>(SDL_sinf(angle) * radius);

        float outline_flag = sin(progress*4.0f);
        if (outline_flag > 0.0f)
        {
            MyScratch->DrawCircle(ox, oy, 5, RGB{ 255, 255, 0, 255 });
            MyScratch->DrawFilledCircle(ox, oy, 3, RGB{ 255, 255, 0, 255 });

        }
        else {
            MyScratch->DrawCircle(ox, oy, 4, RGB{ 255, 255, 0, 160 });

        }
    }
}

void GameAthenaSlashEmUp::DrawReticle(vec3d pos, int radius, float progress)
{
    vec3d n_loc = MyScratch->Get2DPointInFromSpace(pos);
    DrawReticle((int)n_loc.x, (int)n_loc.y, radius, progress);

    //------------------------------
   //TEXT AT 3D LOCATION
   //------------------------------
    vec3d textCoordinates2D = MyScratch->Get2DPointInFromSpace(vec3d{ 2.0f,0.0f,-0.25f });
    MyScratch->DrawText((int)n_loc.x+5, (int)n_loc.y+5, { 0, 255, 0, 255, }, "Target!", MyTextSprites, typingEffect * 2.0f);


}

int GameAthenaSlashEmUp::CreateActor(const Actor& a)
{
    AllActors.push_back(a);
    return static_cast<int>(AllActors.size() - 1);
}

void GameAthenaSlashEmUp::DrawActor(const Actor& a)
{
    MyScratch->DifferDrawMesh(a.m, a.loc, a.rot, a.scale, true);
    AddTargetableLOC(vec3d{ 0.0f,-1.0f,0.0f });
}

void GameAthenaSlashEmUp::DrawActorsFromList(const std::vector<Actor>& actors, bool DrawDifferNow)
{
   
    for (int i = 0; i < actors.size(); ++i)
    {
        MyScratch->MeshColor = actors[i].meshColor;
        MyScratch->DifferDrawMesh(actors[i].m, actors[i].loc, actors[i].rot, actors[i].scale,true);
    }
    if(DrawDifferNow)MyScratch->DrawSortedDifferedMeshes();
}

bool GameAthenaSlashEmUp::IsColliding(vec3d a, vec3d b, float radius)
{
    int x1 = MyScratch->Get2DPointInFromSpace(a).x;
    int y1 = MyScratch->Get2DPointInFromSpace(a).y;
    int x2 = MyScratch->Get2DPointInFromSpace(b).x;
    int y2 = MyScratch->Get2DPointInFromSpace(b).y;

   


    float radius_squared = radius * radius;
    float dist = MyScratch->SquaredDistance(a, b);
    if (dist < radius_squared)
    {
        if (DEBUG_DrawColliders) {
            MyScratch->DrawCircle(x1, y1, (int)(radius*10), RGB{ 0, 255, 0, 255 });
            MyScratch->DrawCircle(x2, y2, (int)(radius * 10), RGB{0, 255, 0, 255});
        }
        return true;
    }
    if (DEBUG_DrawColliders) {
      MyScratch->DrawCircle(x1, y1, (int)(radius * 10), RGB{255, 0, 0, 255});
        MyScratch->DrawCircle(x2, y2, (int)(radius * 10), RGB{ 255, 0, 0, 255 });
    }
    return false;
}

bool GameAthenaSlashEmUp::IsColliding2D(vec3d a, vec3d b, float radius)
{


    return IsColliding(vec3d{ a.x,0,a.z }, vec3d{ b.x,0.0f,b.z }, radius);
}



void GameAthenaSlashEmUp::StateMachineHelloWorldTick()
{

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Hello State Machine \n T %.2f D %.2f", sm.TimeInState, sm.StateDeltaTime);   // format however you want
    MyScratch->DrawText(34, 55, { 255,124,13,255 }, buffer, MyTextSprites,1.0f);
    if (sm.TimeInState > 4.0f)
    {
        sm.SetState(-1); //end all state calls
        return;
    }
}

void GameAthenaSlashEmUp::PlayerIdleState()
{
    //TODO: Set rotation of player in the way it currently works inside of this state machine
    if (MyScratch->Input->GetFireOneHold())
    {
        sm.SetState(State_AthenaAttack);
    }
}

void GameAthenaSlashEmUp::PlayerAttackState()
{
    if (sm.TimeInState >= 0.035f)
    {
        sm.TimeInState = 0.0f;
        sm.SubstateValue0++;
        
        if (sm.SubstateValue0 == 10)
        {
            sm.SubstateValue0 = 0;
            CurrentAthenaFrame = PlayerMeshSequence[9];

            sm.SetState(State_AthenaIdle);
            return;
        }
        CurrentAthenaFrame = PlayerMeshSequence[sm.SubstateValue0];
    }
}

void GameAthenaSlashEmUp::DrawStatsAtLocation(Stats& stats, vec3d loc, bool useFull)
{
    vec3d player_position = loc;
    std::string HP = "HP " + std::to_string(stats.hp);
    if (useFull)
    {
        std::string LV = "LV " + std::to_string(stats.lvl);
        std::string MP = "MP " + std::to_string(stats.mp);

        MyScratch->DrawTextDropShadow((int)MyScratch->Get2DPointInFromSpace(player_position).x - 12, (int)MyScratch->Get2DPointInFromSpace(player_position).y - 8, { 255, 255, 255, 255, }, LV.c_str(), MyTextSprites, 1.0f);
        MyScratch->DrawTextDropShadow((int)MyScratch->Get2DPointInFromSpace(player_position).x - 12, (int)MyScratch->Get2DPointInFromSpace(player_position).y, { 0, 255, 0, 255, }, HP.c_str(), MyTextSprites, 1.0f);
        MyScratch->DrawTextDropShadow((int)MyScratch->Get2DPointInFromSpace(player_position).x - 12, (int)MyScratch->Get2DPointInFromSpace(player_position).y + 8, { 0, 0, 255, 255, }, MP.c_str(), MyTextSprites, 1.0f);
        stats.exp += 5;
        if (stats.exp >= 999)
        {
            //TODO: On Level up, display a text box message !
            ReadNewText("LevelUp");
            stats.exp = 0;
            stats.lvl += 1;
            stats.hp += MyScratch->GetNext(1, 4);
            stats.mp += MyScratch->GetNext(0, 2);
            stats.atk += MyScratch->GetNext(1, 2);
            stats.def += MyScratch->GetNext(0,1);
        }
        int expValue = stats.exp;
        std::string expText = "XP " + std::to_string(expValue);
        if (expValue < 100)expText = "XP 0" + std::to_string(expValue);
        MyScratch->DrawTextDropShadow(
            (int)MyScratch->Get2DPointInFromSpace(player_position).x - 12,
            (int)MyScratch->Get2DPointInFromSpace(player_position).y + 16,
            { 255, 0, 255, 255 },
            expText.c_str(),
            MyTextSprites,
            1.0f
        );
    }
    else
    {
        MyScratch->DrawTextDropShadow((int)MyScratch->Get2DPointInFromSpace(player_position).x - 12, (int)MyScratch->Get2DPointInFromSpace(player_position).y, { 255, 0, 0, 255, }, HP.c_str(), MyTextSprites, 1.0f);
    }

}

void GameAthenaSlashEmUp::DrawHUD(float DeltaTime)
{
    
    //------------------------------
    // STATS
    //------------------------------
    DrawStatsAtLocation(PlayerStats, player_position, true);
    //------------------------------
    //TYPE TEXT 
    //------------------------------
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
   

    //------------------------------
    //Reticle
    //------------------------------
    //vec3d TargetPos2D = MyScratch->Get2DPointInFromSpace(vec3d{ 0.0f,-1.0f,0.0f });
    //DrawReticle(int(TargetPos2D.x), (int)TargetPos2D.y, 12.0f, totalTime * 2.0f);
    DrawReticle(GetClosestVectorFromList(player_position, TargetableLocations,30), 12.0f, totalTime * 2.0f);

    //------------------------------
    //FADE IN HERE!
    //------------------------------
    if (working_float > 0.0f) {
        //fade in
        working_float -= 0.5f * DeltaTime;
        MyScratch->SetFade({ 0,0,0,0 }, working_float);
        CircleTransition((1.0f - working_float) * 1.5f);

    }
    else
    {
        
       // textBoxProgressTick += (strlen(previous_text) + 1) / 4.0f * DeltaTime; //factor size of text in to how fast it takes to print. add 1 so never mult by 0.
        textBoxProgressTick += 2.5f * DeltaTime; //factor size of text in to how fast it takes to print. add 1 so never mult by 0.
       
        //TEXTBOX
       //Disable text for now: 
        //NOTE THIS MUST TICK EVERY FRRAME FOR THE LEVEL UP MESSAGE TO DRAW!
       TextBoxDraw(Reader->GetStringFromSheetTag(RequestedText));
       
    }
}

void GameAthenaSlashEmUp::ClearTargetableLocAry(vec3d* TargetableList, int size_of_array)
{
    currentTargetable = 0;
    for (int i = 0; i < size_of_array; ++i)
    {
        TargetableList[i] = vec3d{0,0,0};
    }
}

void GameAthenaSlashEmUp::AddTargetableLOC(vec3d* TargetableList, int size_of_array, vec3d target)
{
    if (currentTargetable > size_of_array)return;
    TargetableList[currentTargetable] = target;
    currentTargetable++;
}

void GameAthenaSlashEmUp::AddTargetableLOC(vec3d target)
{
    AddTargetableLOC(TargetableLocations, 30, target);

}

vec3d GameAthenaSlashEmUp::GetClosestVectorFromList(vec3d TestAgainst, vec3d* TargetableList, int size_of_array)
{
    vec3d result = { 0,0,0 };
    float last_dist = 1000000.0f;
    for (int i = 0; i < size_of_array; ++i)
    {
        float squared_dist = MyScratch->SquaredDistance2D(TargetableList[i], TestAgainst);
        if (squared_dist < last_dist)
        {
            last_dist = squared_dist;
            result = TargetableList[i];
        }
    }
    return result;
}

void GameAthenaSlashEmUp::AccumulatedBlur(float strength)
{
    MyScratch->BlendBuffers(BlurBuffer, 1.0f - strength); //smaller number == more blur!
    //For crazy SaGa Frontier style mix
    //MyScratch->AddBuffers(BlurBuffer); //smaller number == more blur!
    MyScratch->CopyBufferToBuffer(MyScratch->MainSpace, BlurBuffer);
    
}



GameAthenaSlashEmUp::~GameAthenaSlashEmUp()
{
    delete MyScratch;
    MyScratch = nullptr;
    delete MyTextSprites;
    MyTextSprites = nullptr;
    delete Reader;
    Reader = nullptr;
}