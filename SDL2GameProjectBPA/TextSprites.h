#pragma once

#include "DrawScratchSpace.h"



class TextSprites {
public:
    int R = 255;
    int G = 255;
    int B = 255;

    Sprite Get_A()
    {
        static RGB data[36] = {
            // Row 0 (6 pixels)
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 3
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}
        };
        return ToSprite(data);
    }
    Sprite Get_B()
    {
        static RGB data[36] = {
            // Row 0 (6 pixels)
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},
        };
        return ToSprite(data);
    }
    Sprite ToSprite(RGB data[36])
    {
        Sprite result;
        result.pixels = data;
        result.width = 6;
        result.height = 6;
        return result;
    }

};