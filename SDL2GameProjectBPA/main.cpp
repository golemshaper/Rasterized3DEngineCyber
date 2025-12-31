#include <SDL3/SDL.h>
#include <iostream>


#include "DrawScratchSpace.h"
#include "Game.h"

//
Game* TheGame = new Game();
Uint32 lastTicks = SDL_GetTicks();  // Store this at the end of your previous frame


const int PIXEL_SCALE = 8;
//CHANGE HERE TO MATCH SCRATCH SIZE
const int SCREEN_NATIVE_X = 240;
const int SCREEN_NATIVE_Y = 240;// 160;
//const int SCREEN_NATIVE_X = 135; //youtube shorts aspect
//const int SCREEN_NATIVE_Y = 240;// 160;

const int SCREEN_WIDTH = SCREEN_NATIVE_X * PIXEL_SCALE;
const int SCREEN_HEIGHT = SCREEN_NATIVE_Y * PIXEL_SCALE;


void DrawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    const int sides = 64; // More sides = smoother circle
    for (int i = 0; i < sides; ++i) {
        float theta1 = 2.0f * SDL_PI_F * i / sides;
        float theta2 = 2.0f * SDL_PI_F * (i + 1) / sides;

        int x1 = centerX + static_cast<int>(radius * SDL_cosf(theta1));
        int y1 = centerY + static_cast<int>(radius * SDL_sinf(theta1));
        int x2 = centerX + static_cast<int>(radius * SDL_cosf(theta2));
        int y2 = centerY + static_cast<int>(radius * SDL_sinf(theta2));

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

int main(int argc, char* argv[]) {
    
    //RESET SCRATCH
   // MyScratch->Initialize();
    TheGame->Initialize();
    //TheGame->MyScratch->Initialize();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background

        //GAME UPDATE
       
        // At the start of your current frame
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastTicks) / 1000.0f;  // Convert to seconds
        lastTicks = currentTicks;
        

        TheGame->Tick(deltaTime);
    
        //SCAN LINE DATA
        bool ScanLineOn = false;
        int ColorChangeTwo = 0;
        RGB ScanLineColor = { 0,0,16 };
        RGB ScanLineColor2 = {18,0,0 };
        float TrueScanlineHeight = 0.75f;
        //DRAW
        for (int y = 0; y < SCREEN_NATIVE_Y; y+=1)  //do y+=2 for scan lines
        {
            ScanLineOn = !ScanLineOn;
            ColorChangeTwo++;
            for (int x = 0; x < SCREEN_NATIVE_X; ++x) {
                ScanLineOn = !ScanLineOn;


                int index = y * SCREEN_NATIVE_X + x;
                RGB color = TheGame->MyScratch->MainSpace[index];
                if (ScanLineOn)
                {
                    RGB LocalColor = ScanLineColor;
                    if (ColorChangeTwo>=2)
                    {
                        LocalColor = ScanLineColor2;
                        ColorChangeTwo = 0;
                    }

                    //apply dither:
                    color = color + LocalColor ;
                    
                }
                //Fix clip
                if (color.r > 255)color.r = 255;
                if (color.g > 255)color.g = 255;
                if (color.b > 255)color.b = 255;
                if (color.r < 0)color.r = 0;
                if (color.g < 0)color.g = 0;
                if (color.b < 0)color.b = 0;

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

               // SDL_RenderPoint(renderer, x, y); // SDL3 renamed this from SDL_RenderDrawPoint

                SDL_FRect pixelRect = {
                    static_cast<float>(x * PIXEL_SCALE),
                    static_cast<float>(y * PIXEL_SCALE),
                    static_cast<float>(PIXEL_SCALE),
                    static_cast<float>(PIXEL_SCALE* TrueScanlineHeight)
                };

                SDL_RenderFillRect(renderer, &pixelRect); 
            }
        }


        // update the screen
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS cap
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
