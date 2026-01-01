
#include "Game.h"
#include "DrawScratchSpace.h"
//#include "MonkeyMesh.h"
//#include <cmath>
//#include <SDL3/SDL_mouse.h>
#include "TextSprites.h"

void Game::Initialize()
{
    MyScratch = new DrawScratchSpace();
    MyScratch->Initialize();
    MyTextSprites = new TextSprites();
}
void Game::Tick(float DeltaTime)
{
    totalTime += DeltaTime;
    MyScratch->Clear(RGB{ 255,2,8 });

}
