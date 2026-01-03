#include "InputWraper.h"
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_events.h>

void InputWraper::Prepare()
{
    lx = 0.0f;
    ly = 0.0f;
}

void InputWraper::Tick(float DeltaTime)
{
    // Make sure SDL updates the keyboard state
    SDL_PumpEvents();

    int numkeys = 0;
    const bool* keyboard = SDL_GetKeyboardState(&numkeys);

    if (keyboard[SDL_SCANCODE_W]) {
        // W is held
    }

    if (keyboard[SDL_SCANCODE_SPACE]) {
        // Space is held
    }

    //flush movement
    float grow = 30.0f;
    float shrink = 10.0f;
    lx = Lerp(lx, 0.0f, shrink * DeltaTime);
    ly = Lerp(ly, 0.0f, shrink * DeltaTime);



    //Walking input
    if (keyboard[SDL_SCANCODE_A])
    {
        //lx = -1.0f;
        lx = Lerp(lx, -1.0f, grow * DeltaTime);
    }
    
    if (keyboard[SDL_SCANCODE_D])
    {
        //lx = 1.0f;
        lx = Lerp(lx, 1.0f, grow * DeltaTime);
    }
    
    if (keyboard[SDL_SCANCODE_W])
    {
       // ly = 1.0f;
        ly = Lerp(ly, 1.0f, grow * DeltaTime);
    }
    if (keyboard[SDL_SCANCODE_S])
    {
        //ly = -1.0f;
        ly = Lerp(ly, -1.0f, grow * DeltaTime);
    }
    
}

float InputWraper::GetMovementX()
{
    return lx;
}

float InputWraper::GetMovementY()
{
    return ly;
}

float InputWraper::Lerp(float a, float b, float c)
{
    return a + (b - a) * c;
}
