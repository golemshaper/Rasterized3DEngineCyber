#pragma once

#include "DrawScratchSpace.h"
#include <unordered_map>
#include <ctype.h>



class TextSprites {
public:
   

    static Sprite Get_A()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        //based on https://fontstruct.com/fontstructions/show/821131/6x6_pixel_font
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
    static Sprite Get_B()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

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
    static Sprite Get_C()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},
        };
        return ToSprite(data);
    }
    static Sprite Get_D()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},
        };
        return ToSprite(data);
    }
    static Sprite Get_E()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 2
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 5
           {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},
        };
        return ToSprite(data);
    }
    static Sprite Get_F()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 2
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 3
             {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 5
           {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
        };
        return ToSprite(data);
    }
    static Sprite Get_G()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},
        };
        return ToSprite(data);
    }
    static Sprite Get_H()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 1
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 2
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},

            // Row 3
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 4
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},

            // Row 5
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_I()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            // Row 0 (6 pixels)
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},

            // Row 1
            {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0},

            // Row 2
            {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0},

            // Row 3
            {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0},

            // Row 4
            {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0},

            // Row 5
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0}
        };
        return ToSprite(data);
    }
    //-----------------
    static Sprite Get_J()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B},
            {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {0,0,0}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_K()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {R,G,B}, {R,G,B}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0},
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {R,G,B}, {R,G,B}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {R,G,B}, {R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_L()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
            {R,G,B}, {R,G,B}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0},
            {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}, {R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_M()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{R,G,B},{0,0,0},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_N()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_O()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_P()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_Q()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_R()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_S()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_T()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_U()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_V()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_W()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {


           {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
           {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
           {R,G,B},{R,G,B},{0,0,0},{R,G,B},{0,0,0},{R,G,B},
           {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
           {R,G,B},{R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},
           {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
           
        };
        return ToSprite(data);
    }
    static Sprite Get_X()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_Y()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_Z()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    //---
    static Sprite Get_0()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }
    static Sprite Get_1()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    
    static Sprite Get_2()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }
    static Sprite Get_3()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_4()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }

    static Sprite Get_5()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_6()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_7()
    {
        const int R = 255;
        const int G = 255;

        const int B = 255;
        static RGB data[36] = {
            {R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_8()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_9()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_Exclamation()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_Space()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }

    static Sprite Get_Question()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {0,0,0},{R,G,B},{R,G,B},{R,G,B},{R,G,B},{0,0,0},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{R,G,B},{0,0,0},{0,0,0}
        };
        return ToSprite(data);
    }


    static Sprite Get_Percent()
    {
        const int R = 255;
        const int G = 255;
        const int B = 255;

        static RGB data[36] = {
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},
            {R,G,B},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{0,0,0},
            {0,0,0},{0,0,0},{0,0,0},{R,G,B},{0,0,0},{0,0,0},
            {0,0,0},{0,0,0},{R,G,B},{0,0,0},{0,0,0},{0,0,0},
            {0,0,0},{R,G,B},{0,0,0},{0,0,0},{R,G,B},{R,G,B},
            {R,G,B},{0,0,0},{0,0,0},{0,0,0},{R,G,B},{R,G,B}
        };
        return ToSprite(data);
    }



    static Sprite ToSprite(RGB data[36])
    {
        Sprite result;
        result.pixels = data;
        result.width = 6;
        result.height = 6;
        return result;
    }

    //---
    typedef Sprite(*SpriteFunc)();

    
    inline Sprite GetSpriteForChar(char c)
    {
        static const std::unordered_map<char, Sprite(*)()> map = {
            {'A', TextSprites::Get_A},
            {'B', TextSprites::Get_B},
            {'C', TextSprites::Get_C},
            {'D', TextSprites::Get_D},
            {'E', TextSprites::Get_E},
            {'F', TextSprites::Get_F},
            {'G', TextSprites::Get_G},
            {'H', TextSprites::Get_H},
            {'I', TextSprites::Get_I},
            {'J', TextSprites::Get_J},
            {'K', TextSprites::Get_K},
            {'L', TextSprites::Get_L},
            {'M', TextSprites::Get_M},
            {'N', TextSprites::Get_N},
            {'O', TextSprites::Get_O},
            {'P', TextSprites::Get_P},
            {'Q', TextSprites::Get_Q},
            {'R', TextSprites::Get_R},
            {'S', TextSprites::Get_S},
            {'T', TextSprites::Get_T},
            {'U', TextSprites::Get_U},
            {'V', TextSprites::Get_V},
            {'W', TextSprites::Get_W},
            {'X', TextSprites::Get_X},
            {'Y', TextSprites::Get_Y},
            {'Z', TextSprites::Get_Z},

            {'0', TextSprites::Get_0},
            {'1', TextSprites::Get_1},
            {'2', TextSprites::Get_2},
            {'3', TextSprites::Get_3},
            {'4', TextSprites::Get_4},
            {'5', TextSprites::Get_5},
            {'6', TextSprites::Get_6},
            {'7', TextSprites::Get_7},
            {'8', TextSprites::Get_8},
            {'9', TextSprites::Get_9},

            {'!', TextSprites::Get_Exclamation},
            {'?', TextSprites::Get_Question},
            {'%', TextSprites::Get_Percent},
        };

        c = toupper(c); // optional, if you want case-insensitivity

        auto it = map.find(c);
        if (it != map.end())
            return it->second(); // call the function

        // fallback: return space
        return Get_Space();
    }


};