#include "DrawScratchSpace.h"
#include <random>
#include <algorithm>
#include "TextSprites.h"
//#include "BMPReader.hpp"


void DrawScratchSpace::SetTexture(RGB* tex, int w, int h)
{
    TextureBufferW = w;
    TextureBufferH = h;
    TextureBuffer = tex;
}

void DrawScratchSpace::MoveMainspaceToExtraBuffer()
{
    //Take whatever is on the screen, and back it up in the extra buffer so you can clear it and draw new stuff to add or multiply later
    std::copy(std::begin(MainSpace), std::end(MainSpace), ExtraBuffer);
}

void DrawScratchSpace::MultiplyBuffers() 
{
    for (int i = 0; i < TOTAL_PIXELS; ++i) 
    {
        MainSpace[i] = (MainSpace[i] * ExtraBuffer[i])/255;
    }
   
}

void DrawScratchSpace::AddBuffers()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = MainSpace[i] + ExtraBuffer[i];
    }
}

void DrawScratchSpace::AddBuffers(RGB* from)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = MainSpace[i] + from[i];
    }
}

void DrawScratchSpace::AverageBuffers()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = (MainSpace[i] + ExtraBuffer[i])/2;
    }
}

void DrawScratchSpace::InvertBuffer()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = RGB{ 255,255,255,255 } - MainSpace[i];
    }
}

void DrawScratchSpace::ClearZBufffer()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        //255 is "max color, but we can go over that. set to 1024 so we have pleanty of depth to work with. Z is actually the inverse of how it draws.
        //higher values are deeper
        ZBuffer[i] = RGB{ 1024, 1024, 1024 ,255 };
    }
}

void DrawScratchSpace::DrawZBufffer()
{
   // 

    //for (int i = 0; i < TOTAL_PIXELS; ++i)
    //{
    //    //normalize
    //    ZBuffer[i].r = (ZBuffer[i].r * 255 + 512) / 1024;
    //    ZBuffer[i].g = (ZBuffer[i].g * 255 + 512) / 1024;
    //    ZBuffer[i].b = (ZBuffer[i].b * 255 + 512) / 1024;
    //}
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = RGB{ 255,255,255,255 } - ZBuffer[i];
    }
    //BrightnessContrastOnBuffer(MainSpace, 0.0f, 2.5f);
}

void DrawScratchSpace::DrawZBuffferMaskSky()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        if (ZBuffer[i].r >= 1024)continue;
        MainSpace[i] = RGB{ 255,255,255,255 } - ZBuffer[i];
    }
}

void DrawScratchSpace::PushBackDepthBuffer(int amount)
{
    //take the current depth and push it all back by some amount. negative to pull it forward.
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        ZBuffer[i] = ZBuffer[i] + RGB{ amount,amount,amount,0 };
    }
}

void DrawScratchSpace::ApplyMask()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        if (MainSpace[i].a ==255)
        {
            MainSpace[i] = ExtraBuffer[i];
        }
       // MainSpace[i] = ExtraBuffer[i];
    }

}

void DrawScratchSpace::BlendBuffers(float amount)
{
    amount = 1.0f - amount;
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {

        MainSpace[i] = Lerp(MainSpace[i] , ExtraBuffer[i] ,amount);
    }
}

void DrawScratchSpace::BlendBuffers(RGB* from, float amount)
{
    amount = 1.0f - amount;
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {

        MainSpace[i] = Lerp(MainSpace[i], from[i], amount);
    }
}

void DrawScratchSpace::CopyBufferToBuffer(RGB* from, RGB* to)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {

        to[i] = from[i];
    }
}

RGB DrawScratchSpace::SampleTexture(const RGB* tex, int texW, int texH, float u, float v)
{
    //clamp
   /* if (u < 0.0f) u = 0.0f;
    if (u > 1.0f) u = 1.0f;
    if (v < 0.0f) v = 0.0f;
    if (v > 1.0f) v = 1.0f;*/
    // wrap UVs into [0,1)
    u = u - floorf(u);
    v = v - floorf(v);


    int x = int(u * (texW - 1));
    int y = int(v * (texH - 1));

    return tex[y * texW + x];
}

/// <summary>
/// Only call me once. Use Clear() after if you want to clear the screen buffer
/// </summary>
/// <param name="wipe"></param>
void DrawScratchSpace::Initialize()
{
    Initialize(RGB{ 0,0,0 });
    Input = new InputWraper();

   /*
   THIS IS HOW WE WILL LOAD TEXTURES LATER!
   int w = 32, h = 32;
    RGB* TestTexture = ReadBMP("Assets/HelloBitmap.bmp", w, h);
    TextureBuffer = TestTexture;*/
}
/// <summary>
/// Only call me once. Use Clear() after if you want to clear the screen buffer
/// </summary>
/// <param name="wipe"></param>
void DrawScratchSpace::Initialize(RGB wipe)
{
    Clear(wipe);
   
    //Setup Mesh
    MeshCube.Tris = {

        // SOUTH
       triangle { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
       triangle { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

        // EAST                                                      
        triangle{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        triangle{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

        // NORTH                                                     
        triangle{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        triangle{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

        // WEST                                                      
        triangle{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        triangle{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

        // TOP                                                       
        triangle{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        triangle{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

        // BOTTOM                                                    
        triangle{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        triangle{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

    };
   


    // Projection Matrix
    float fNear = NearClip; //0.1f
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = (float)SCREEN_Y / (float)SCREEN_X;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    MatrixProj.m[0][0] = fAspectRatio * fFovRad;
    MatrixProj.m[1][1] = fFovRad;
    MatrixProj.m[2][2] = fFar / (fFar - fNear);
    MatrixProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    MatrixProj.m[2][3] = 1.0f;
    MatrixProj.m[3][3] = 0.0f;

    //Font Stuff
    //Look at how OliveC does the font...https://github.com/tsoding/olive.c/blob/master/olive.c


    //RGB MissingTexture[64] = {
    //    // Row 0
    //    {0,0,0}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150},
    //    // Row 1
    //    {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150},
    //    // Row 2
    //    {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220},
    //    // Row 3
    //    {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220},
    //    // Row 4
    //    {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150},
    //    // Row 5
    //    {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150},
    //    // Row 6
    //    {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220},
    //    // Row 7
    //    {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}, {140,140,150}, {140,140,150}, {180,200,220}, {180,200,220}
    //};
    RGB MissingTexture[64] = {
        {0,0,0}, {255,255,255},{0,0,0}, {255,255,255},
        {0,0,0}, {255,255,255},{0,0,0}, {255,255,255},
        {0,0,0}, {255,255,255},{0,0,0}, {255,255,255},
        {0,0,0}, {255,255,255},{0,0,0}, {255,255,255},
    };

    SetTexture(MissingTexture, 4, 4);
}

void DrawScratchSpace::Clear()
{
    Clear(RGB{ 0,0,0 });
}

void DrawScratchSpace::Clear(RGB wipe)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = wipe;
    }
}

inline int DrawScratchSpace::SinglePixelBrightContrast(int c, float brightness, float contrast)
{
    float x = c / 255.0f;

    x = (x - 0.5f) * contrast + 0.5f;

    x += brightness;

    // Clamp and convert back
    x = std::min(std::max(x, 0.0f), 1.0f);
    return (int)(x * 255.0f + 0.5f);
}

void DrawScratchSpace::BrightnessContrastOnBuffer(RGB* buffer, float brightness, float contrast)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        if (buffer[i].r < 0)buffer[i].r = 0;
        if (buffer[i].g< 0)buffer[i].g = 0;
        if (buffer[i].b < 0)buffer[i].b = 0;
        



        buffer[i].r = SinglePixelBrightContrast(buffer[i].r, brightness, contrast);
        buffer[i].g = SinglePixelBrightContrast(buffer[i].g, brightness, contrast);
        buffer[i].b = SinglePixelBrightContrast(buffer[i].b, brightness, contrast);
    }
}

void DrawScratchSpace::RandomScreenFill()
{
    //replace with random function?
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i].r = dist(gen);
        MainSpace[i].g = dist(gen);
        MainSpace[i].b = dist(gen);
    }
}

void DrawScratchSpace::RandomScreenFill(RGB* buffer)
{
    //replace with random function?
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        buffer[i].r = dist(gen);
        buffer[i].g = dist(gen);
        buffer[i].b = dist(gen);
    }
}

void DrawScratchSpace::DrawRectangle(int x, int y, int width, int height, RGB color)
{
    for (int dy = 0; dy < height; ++dy) {
        for (int dx = 0; dx < width; ++dx) {
            int px = x + dx;
            int py = y + dy;

            // Bounds check
            if (px >= 0 && px < SCREEN_X && py >= 0 && py < SCREEN_Y) {
                int index = py * SCREEN_X + px;
                RGB& dst = MainSpace[index];
                MainSpace[index] = AlphaBlend(dst, color);
            }
        }
    }
}

void DrawScratchSpace::DrawRectangle(
    int x, int y,
    int width, int height,
    RGB topLeft,
    RGB topRight,
    RGB bottomLeft,
    RGB bottomRight)
{
    for (int dy = 0; dy < height; ++dy)
    {
        float ty = (height > 1) ? (float)dy / (float)(height - 1) : 0.0f;

        for (int dx = 0; dx < width; ++dx)
        {
            float tx = (width > 1) ? (float)dx / (float)(width - 1) : 0.0f;

            int px = x + dx;
            int py = y + dy;

            if (px >= 0 && px < SCREEN_X && py >= 0 && py < SCREEN_Y)
            {
                // Interpolate top row
                RGB top;
                top.r = (uint8_t)((1 - tx) * topLeft.r + tx * topRight.r);
                top.g = (uint8_t)((1 - tx) * topLeft.g + tx * topRight.g);
                top.b = (uint8_t)((1 - tx) * topLeft.b + tx * topRight.b);
                top.a = (uint8_t)((1 - tx) * topLeft.a + tx * topRight.a);
                // Interpolate bottom row
                RGB bottom;
                bottom.r = (uint8_t)((1 - tx) * bottomLeft.r + tx * bottomRight.r);
                bottom.g = (uint8_t)((1 - tx) * bottomLeft.g + tx * bottomRight.g);
                bottom.b = (uint8_t)((1 - tx) * bottomLeft.b + tx * bottomRight.b);
                bottom.a = (uint8_t)((1 - tx) * bottomLeft.a + tx * bottomLeft.a);
                // Interpolate vertically between top and bottom
                RGB finalColor;
                finalColor.r = (uint8_t)((1 - ty) * top.r + ty * bottom.r);
                finalColor.g = (uint8_t)((1 - ty) * top.g + ty * bottom.g);
                finalColor.b = (uint8_t)((1 - ty) * top.b + ty * bottom.b);
                finalColor.a = (uint8_t)((1 - ty) * top.a + ty * bottom.a);

                int index = py * SCREEN_X + px;
                RGB& dst = MainSpace[index];
                MainSpace[index] = AlphaBlend(dst, finalColor);
            }
        }
    }
}

void DrawScratchSpace::DrawSquare(int x, int y, int size, RGB color) {
    for (int dy = 0; dy < size; ++dy) {
        for (int dx = 0; dx < size; ++dx) {
            int px = x + dx;
            int py = y + dy;

            // Bounds check
            if (px >= 0 && px < SCREEN_X && py >= 0 && py < SCREEN_Y) {
                int index = py * SCREEN_X + px;
                //MainSpace[index] = color;
                RGB& dst = MainSpace[index];
                MainSpace[index] = AlphaBlend(dst, color);

            }
        }
    }
}

void DrawScratchSpace::DrawSquareMultiply(int x, int y, int size, RGB color)
{
    for (int dy = 0; dy < size; ++dy) {
        for (int dx = 0; dx < size; ++dx) {
            int px = x + dx;
            int py = y + dy;

            // Bounds check
            if (px >= 0 && px < SCREEN_X && py >= 0 && py < SCREEN_Y) {
                int index = py * SCREEN_X + px;
                MainSpace[index] = MainSpace[index] * color;
                
            }
        }
    }
}
void DrawScratchSpace::DrawTriangle(Point p0, Point p1, Point p2, RGB color) {

    


    // Sort points by y-coordinate (ascending)
    if (p1.y < p0.y) std::swap(p0, p1);
    if (p2.y < p0.y) std::swap(p0, p2);
    if (p2.y < p1.y) std::swap(p1, p2);

    auto edgeInterp = [](int y, Point a, Point b) -> int {
        if (b.y == a.y) return a.x;
        return a.x + (b.x - a.x) * (y - a.y) / (b.y - a.y);
    };

    for (int y = p0.y; y <= p2.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        bool upper = y < p1.y || p1.y == p0.y;
        int xa = upper ? edgeInterp(y, p0, p1) : edgeInterp(y, p1, p2);
        int xb = edgeInterp(y, p0, p2);

        if (xa > xb) std::swap(xa, xb);

        for (int x = xa; x <= xb; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            MainSpace[y * SCREEN_X + x] = color;
        }
    }
}
void DrawScratchSpace::DrawTriangle(Vertex v0, Vertex v1, Vertex v2)
{
    DrawTriangle(v0, v1, v2, 255);
}
void DrawScratchSpace::DrawTriangle(Vertex v0, Vertex v1, Vertex v2, int z)
{
    //const RGB* TestTexture = TextureBuffer;  //USE THE TEXTURE FROM THE TEXTURE BUFFER.
    const RGB* CurrentTexture = TextureBuffer;  //USE THE TEXTURE FROM THE TEXTURE BUFFER.
    //I DISABLED IT, BUT IT CAN BE LOADED FROM THE TEXTURE BUFFER.
    //I NEED UVs BEFORE I PROCEDE WITH THIS!


//TEXTURE PLACEHOLDER



    // Sort vertices by y-coordinate
    if (v1.y < v0.y) std::swap(v0, v1);
    if (v2.y < v0.y) std::swap(v0, v2);
    if (v2.y < v1.y) std::swap(v1, v2);

    auto interpolate = [](int y, const Vertex& a, const Vertex& b) -> Vertex {
        if (b.y == a.y) return a;
        float t = float(y - a.y) / float(b.y - a.y);

        Vertex out;
        out.x = (int)(a.x + t * (b.x - a.x));
        out.y = y;

        out.color.r = (int)(a.color.r + t * (b.color.r - a.color.r));
        out.color.g = (int)(a.color.g + t * (b.color.g - a.color.g));
        out.color.b = (int)(a.color.b + t * (b.color.b - a.color.b));

        out.u = a.u + t * (b.u - a.u);
        out.v = a.v + t * (b.v - a.v);

        return out;
    };

    // First, draw the top part of the triangle (from v0 to v1)
    for (int y = v0.y; y < v1.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        Vertex va = interpolate(y, v0, v1);
        Vertex vb = interpolate(y, v0, v2);


       


        if (va.x > vb.x) std::swap(va, vb);

        for (int x = va.x; x <= vb.x; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            float t = (vb.x == va.x) ? 0.0f : static_cast<float>(x - va.x) / (vb.x - va.x);

            RGB color = {
                static_cast<int>(va.color.r + t * (vb.color.r - va.color.r)),
                static_cast<int>(va.color.g + t * (vb.color.g - va.color.g)),
                static_cast<int>(va.color.b + t * (vb.color.b - va.color.b))
            };


          


            if (ZWriteOn && ZBuffer[y * SCREEN_X + x].r < z)
            {
                //don't draw hidden triangle parts!
                continue;
            }
            if (ZWriteOn)
            {
                ZBuffer[y * SCREEN_X + x] = RGB{ z,z,z,255 };
            }

            // UV CALC 
            float tX = (vb.x == va.x) ? 0.0f : float(x - va.x) / float(vb.x - va.x);
            float u = Lerp((float)va.u, (float)vb.u, tX);
            float v = Lerp((float)va.v, (float)vb.v, tX);
            RGB textured = SampleTexture(CurrentTexture, TextureBufferW, TextureBufferH, u, v);

            if (DrawHighlightEdgeOnly == true && TextureOnForEdgelight == false)
            {
                textured = RGB_White;
            }
            if (TextureDrawOn) {

                //TODO: Instead of overriding, have a blend by alpha option! Also consider getting rid of the non-multiply texture opti
                if (!MultiplyInTextureMode)
                {
                    MainSpace[y * SCREEN_X + x] = (color + textured) / 2;

                }
                else
                {
                    MainSpace[y * SCREEN_X + x] = (color * textured) / 255;

                }
            }
            else
            {
                MainSpace[y * SCREEN_X + x] = color;
            }
        }
    }

    // Now, draw the bottom part of the triangle (from v1 to v2)
    for (int y = v1.y; y <= v2.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        Vertex va = interpolate(y, v1, v2);
        Vertex vb = interpolate(y, v0, v2);

        if (va.x > vb.x) std::swap(va, vb);

        for (int x = va.x; x <= vb.x; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            float t = (vb.x == va.x) ? 0.0f : static_cast<float>(x - va.x) / (vb.x - va.x);

            RGB color = {
                static_cast<int>(va.color.r + t * (vb.color.r - va.color.r)),
                static_cast<int>(va.color.g + t * (vb.color.g - va.color.g)),
                static_cast<int>(va.color.b + t * (vb.color.b - va.color.b))
            };
            if (ZWriteOn && ZBuffer[y * SCREEN_X + x].r < z)
            {
                //don't draw hidden triangle parts!
                continue;
            }
            if (ZWriteOn) 
            {
                ZBuffer[y * SCREEN_X + x] = RGB{ z,z,z,255 };
            }


            // UV CALC 
            float tX = (vb.x == va.x) ? 0.0f : float(x - va.x) / float(vb.x - va.x);
            float u = Lerp((float)va.u, (float)vb.u, tX);
            float v = Lerp((float)va.v, (float)vb.v, tX);

             RGB textured = SampleTexture(CurrentTexture, TextureBufferW, TextureBufferH, u, v);
            //RGB textured2 = SampleTexture(TestTexture, 8, 8, u, v);
            if (TextureDrawOn) {

                //MainSpace[y * SCREEN_X + x] = (color + textured) / 2;
                if (!MultiplyInTextureMode)
                {
                    MainSpace[y * SCREEN_X + x] = (color + textured) / 2;

                }
                else
                {
                    MainSpace[y * SCREEN_X + x] = (color * textured) / 255;
                }
            }
            else
            {
                MainSpace[y * SCREEN_X + x] = color;
            }
        }
    }
}
void DrawScratchSpace::DrawTriangleToZBuffer(Vertex v0, Vertex v1, Vertex v2, int z)
{
    //Double check, but I don't think we need this anymore
    if (ZWriteOn == false)
    {
        return;
    }
    //FIND A WAY TO NOT NEED TO DUPLICATE THIS CODE (MAYBE PASS THE BUFFER IN AS ARGS)
    // Sort vertices by y-coordinate
    if (v1.y < v0.y) std::swap(v0, v1);
    if (v2.y < v0.y) std::swap(v0, v2);
    if (v2.y < v1.y) std::swap(v1, v2);

    auto interpolate = [](int y, const Vertex& a, const Vertex& b) -> Vertex {
        if (b.y == a.y) return a;
        float t = static_cast<float>(y - a.y) / (b.y - a.y);
        return {
            static_cast<int>(a.x + t * (b.x - a.x)),
            y,
            {
                static_cast<int>(a.color.r + t * (b.color.r - a.color.r)),
                static_cast<int>(a.color.g + t * (b.color.g - a.color.g)),
                static_cast<int>(a.color.b + t * (b.color.b - a.color.b))
            }
        };
    };

    // First, draw the top part of the triangle (from v0 to v1)
    for (int y = v0.y; y < v1.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        Vertex va = interpolate(y, v0, v1);
        Vertex vb = interpolate(y, v0, v2);
        

        if (va.x > vb.x) std::swap(va, vb);

        for (int x = va.x; x <= vb.x; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            float t = (vb.x == va.x) ? 0.0f : static_cast<float>(x - va.x) / (vb.x - va.x);

            RGB color = {
                static_cast<int>(va.color.r + t * (vb.color.r - va.color.r)),
                static_cast<int>(va.color.g + t * (vb.color.g - va.color.g)),
                static_cast<int>(va.color.b + t * (vb.color.b - va.color.b))
            };
            if (ZBuffer[y * SCREEN_X + x].r < z)
            {
                //don't draw hidden triangle parts!
               // ZBuffer[y * SCREEN_X + x] = RGB{ ZBuffer[y * SCREEN_X + x].r,255,255,255 };
                continue;
            }
            ZBuffer[y * SCREEN_X + x] = RGB{ z,z,z,255 };
        }
    }

    // Now, draw the bottom part of the triangle (from v1 to v2)
    for (int y = v1.y; y <= v2.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        Vertex va = interpolate(y, v1, v2);
        Vertex vb = interpolate(y, v0, v2);

        if (va.x > vb.x) std::swap(va, vb);

        for (int x = va.x; x <= vb.x; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            float t = (vb.x == va.x) ? 0.0f : static_cast<float>(x - va.x) / (vb.x - va.x);

            RGB color = {
                static_cast<int>(va.color.r + t * (vb.color.r - va.color.r)),
                static_cast<int>(va.color.g + t * (vb.color.g - va.color.g)),
                static_cast<int>(va.color.b + t * (vb.color.b - va.color.b))
            };

            if (ZBuffer[y * SCREEN_X + x].r < z)
            {
                //don't draw hidden triangle parts!
                //ZBuffer[y * SCREEN_X + x] = RGB{ ZBuffer[y * SCREEN_X + x].r,255,255,255 };
                continue;
            }
            ZBuffer[y * SCREEN_X + x] = RGB{ z,z,z,255 };
        }
    }
}
void DrawScratchSpace::DrawCircle(int x, int y, int radius, RGB color)
{
    const int sides = 64; // More sides = smoother circle
        for (int i = 0; i < sides; ++i) {
            float theta1 = 2.0f * PI * i / sides;
            float theta2 = 2.0f * PI * (i + 1) / sides;
    
            int x1 = x + static_cast<int>(radius * cos(theta1));
            int y1 = y + static_cast<int>(radius * sin(theta1));
            int x2 = x + static_cast<int>(radius * cos(theta2));
            int y2 = y + static_cast<int>(radius * sin(theta2));
    
            DrawLine(x1, y1, x2, y2, color);
            
        }
}
void DrawScratchSpace::DrawFilledCircle(int x, int y, int radius, RGB color)
{
    for (int dy = -radius; dy <= radius; ++dy)
    {
        int dx = static_cast<int>(SDL_sqrtf(radius * radius - dy * dy));

        int x1 = x - dx;
        int x2 = x + dx;
        int yy = y + dy;

        DrawLine(x1, yy, x2, yy, color);
    }
}
void DrawScratchSpace::DrawTriangleGlitchy(Vertex v0, Vertex v1, Vertex v2)
{
    // Sort vertices by y
    if (v1.y < v0.y) std::swap(v0, v1);
    if (v2.y < v0.y) std::swap(v0, v2);
    if (v2.y < v1.y) std::swap(v1, v2);

    auto interpolate = [](int y, const Vertex& a, const Vertex& b) -> Vertex {
        if (b.y == a.y) return a;
        float t = static_cast<float>(y - a.y) / (b.y - a.y);
        return {
            static_cast<int>(a.x + t * (b.x - a.x)),
            y,
            {
                static_cast<int>(a.color.r + t * (b.color.r - a.color.r)),
                static_cast<int>(a.color.g + t * (b.color.g - a.color.g)),
                static_cast<int>(a.color.b + t * (b.color.b - a.color.b))
            }
        };
    };

    for (int y = v0.y; y <= v2.y; ++y) {
        if (y < 0 || y >= SCREEN_Y) continue;

        bool upper = y < v1.y || v1.y == v0.y;
        Vertex va = upper ? interpolate(y, v0, v1) : interpolate(y, v1, v2);
        Vertex vb = interpolate(y, v0, v2);

        if (va.x > vb.x) std::swap(va, vb);

        for (int x = va.x; x <= vb.x; ++x) {
            if (x < 0 || x >= SCREEN_X) continue;
            float t = (vb.x == va.x) ? 0.0f : static_cast<float>(x - va.x) / (vb.x - va.x);

            RGB color = {
                static_cast<int>(va.color.r + t * (vb.color.r - va.color.r)),
                static_cast<int>(va.color.g + t * (vb.color.g - va.color.g)),
                static_cast<int>(va.color.b + t * (vb.color.b - va.color.b))
            };

            MainSpace[y * SCREEN_X + x] = color;
        }
    }

}
RGB* DrawScratchSpace::ColorizeSpriteData(RGB* data, int spriteWidth, int spriteHeight, RGB color)
{
    int totalPixels = spriteWidth * spriteHeight;

    // Output buffer (static so the pointer stays valid)
    static RGB out[1024]; // enough for any small sprite; or allocate dynamically if needed

    for (int i = 0; i < totalPixels; i++)
    {
        RGB p = data[i];

        // Detect white pixel
        if (p.r == 255 && p.g == 255 && p.b == 255)
        {
            out[i].r = (p.r * color.r) / 255;
            out[i].g = (p.g * color.g) / 255;
            out[i].b = (p.b * color.b) / 255;
            out[i].a = color.a;
        }
        else
        {
            out[i] = p;
        }
    }

    return out;
}
void DrawScratchSpace::DrawSprite(int startX, int startY, Sprite sprite)
{
    DrawSprite(startX,startY,sprite.pixels,sprite.width,sprite.height);
}
void DrawScratchSpace::DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight) {

    RGB Black = { 0,0,0,255 };
    RGB Black2 = { 0,0,0,0 };

    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {

            //Wrap around
            int screenX = (startX + x) % SCREEN_X;
            int screenY = (startY + y) % SCREEN_Y;

            // Handle negative wrapping
            if (screenX < 0) screenX += SCREEN_X;
            if (screenY < 0) screenY += SCREEN_Y;

            int screenIndex = screenY * SCREEN_X + screenX;
            int spriteIndex = y * spriteWidth + x;

            RGB pixel = SpriteData[spriteIndex];
            if (pixel == Black || pixel == Black2) {
                continue;
            }

            MainSpace[screenIndex] = pixel;
        }
    }
}
void DrawScratchSpace::DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight, bool wrap)
{
    RGB Black = { 0,0,0,255 };
    RGB Black2 = { 0,0,0,0 };

    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {

            int screenX = startX + x;
            int screenY = startY + y;

            if (wrap)
            {
                screenX %= SCREEN_X;
                screenY %= SCREEN_Y;

                if (screenX < 0) screenX += SCREEN_X;
                if (screenY < 0) screenY += SCREEN_Y;
            }
            else
            {
                // If not wrapping, skip pixels outside the screen
                if (screenX < 0 || screenX >= SCREEN_X ||
                    screenY < 0 || screenY >= SCREEN_Y)
                {
                    continue;
                }
            }

            int screenIndex = screenY * SCREEN_X + screenX;
            int spriteIndex = y * spriteWidth + x;

            RGB pixel = SpriteData[spriteIndex];
            if (pixel == Black || pixel == Black2)
                continue;

           // MainSpace[screenIndex] = pixel;



            float alpha = pixel.a / 255.0f;
            float invAlpha = 1.0f - alpha;

            MainSpace[screenIndex].r = (int)(pixel.r * alpha + MainSpace[screenIndex].r * invAlpha + 0.5f);
            MainSpace[screenIndex].g = (int)(pixel.g * alpha + MainSpace[screenIndex].g * invAlpha + 0.5f);
            MainSpace[screenIndex].b = (int)(pixel.b * alpha + MainSpace[screenIndex].b * invAlpha + 0.5f);
            // Usually keep destination alpha or set to 255 (opaque framebuffer)
            MainSpace[screenIndex].a = 255;


        }
    }
}
void DrawScratchSpace::DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight, float angle)
{
    RGB Black = { 0, 0, 0 };
    Point pivot = { static_cast<int>(spriteWidth / 2.0f), static_cast<int>(spriteHeight / 2.0f) };

    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {
            Point local = { x, y };
            Point rotated = RotatePoint(local, pivot, angle);

            int screenX = startX + static_cast<int>(rotated.x);
            int screenY = startY + static_cast<int>(rotated.y);

            if (screenX >= 0 && screenX < SCREEN_X && screenY >= 0 && screenY < SCREEN_Y) {
                int spriteIndex = y * spriteWidth + x;
                if (SpriteData[spriteIndex] == Black) continue;

                int screenIndex = screenY * SCREEN_X + screenX;


                float alpha = SpriteData[spriteIndex].a / 255.0;
                // Calculate inverse alpha for efficiency


               
                MainSpace[screenIndex] = SpriteData[spriteIndex];
            }
        }
    }
}
void DrawScratchSpace::DrawSpriteAdd(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight)
{
    RGB Black = { 0,0,0 };
    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {
            int screenX = startX + x;
            int screenY = startY + y;

            if (screenX >= 0 && screenX < SCREEN_X && screenY >= 0 && screenY < SCREEN_Y) {
                int screenIndex = screenY * SCREEN_X + screenX;
                int spriteIndex = y * spriteWidth + x;
                if (SpriteData[spriteIndex] == Black)
                {
                    continue;
                }
                MainSpace[screenIndex] = MainSpace[screenIndex] + SpriteData[spriteIndex];
            }
        }
    }
}
void DrawScratchSpace::DrawTextAtPos(int X, int Y, RGB color, const char* text, TextSprites* tSprites)
{
    DrawTextAtPos(X,Y,color,text,tSprites,1.0f);
}

void DrawScratchSpace::DrawTextAtPos(int X, int Y, RGB color, const char* text, TextSprites* tSprites, float amount_revealed)
{
    
    int cursorX = X;
    int cursorY = Y;

    // Count characters
    int length = 0;
    while (text[length] != '\0')
        length++;

    // Clamp amount_revealed
    if (amount_revealed < 0.0f) amount_revealed = 0.0f;
    if (amount_revealed > 1.0f) amount_revealed = 1.0f;

    // Compute how many characters to show
    int chars_to_show = (int)(length * amount_revealed);

    // Draw only that many characters
    for (int j = 0; j < chars_to_show+1; j++)
    {
        int i = j;
        if (i > chars_to_show)
        {
            i = chars_to_show;
        }
        char c = text[i];
       /* if (j >= chars_to_show)
        {
        //debug cursor
            c = 'x';
        }*/

        if (c == '\n')
        {
            cursorX = X;
            cursorY += 7;
            continue;
        }
        Sprite s = tSprites->GetSpriteForChar(c);
        
        s.pixels = ColorizeSpriteData(s.pixels,s.width,s.height,color);

        int offsetY = 0;
        if (i == chars_to_show-1 && chars_to_show!=length)
        {
            offsetY = -2;
        }
        if (j < chars_to_show) {
            DrawSprite(cursorX, cursorY + offsetY, s.pixels, s.width, s.height, false);
        }

        cursorX += 7; // 6px glyph + 1px spacing
    }
}

void DrawScratchSpace::DrawTextDropShadow(int X, int Y, RGB color, const char* text, TextSprites* tSprites, float amount_revealed)
{
    DrawTextAtPos(X + 1, Y + 1, RGB{1,1,1,175}, text, tSprites, amount_revealed);
    DrawTextAtPos(X, Y, color, text, tSprites, amount_revealed);
}

//Bresenham’s line algorithm
void DrawScratchSpace::DrawLine(int x0, int y0, int x1, int y1, RGB color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < SCREEN_X && y0 >= 0 && y0 < SCREEN_Y) {
            int index = y0 * SCREEN_X + x0;
            //old blend: MainSpace[index] = (MainSpace[index] + color) /2;


            RGB& dst = MainSpace[index];
            MainSpace[index] = AlphaBlend(dst, color);


            //MainSpace[index] = color;
        }

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

void DrawScratchSpace::Draw3DLine(vec3d a, vec3d b, RGB c)
{
    vec3d a2d = Get2DPointInFromSpace(a);
    vec3d b2d = Get2DPointInFromSpace(b);

    DrawLine(a2d.x, a2d.y, b2d.x, b2d.y,c);
}

int DrawScratchSpace::GetRandom(int a, int b)
{
    /*std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);*/
    unsigned int r = GetNext();
    unsigned int range = (b - a + 1);
    unsigned int value = (r >> 24) % range; // use high bits
    return a + value;


    //return GetNext(a, b);
}

float DrawScratchSpace::GetRandomFloat(float a, float b)
{
    unsigned int r = GetNext();
    // Normalize
    float t = (float)r / 4294967295.0f; // UINT32_MAX
    return a + t * (b - a);
}
unsigned int DrawScratchSpace::GetNext()
{
    //https://en.wikipedia.org/wiki/Linear_congruential_generator
    seed = 1103515245u * seed + 12345u;
    return seed;
}

int DrawScratchSpace::GetNext(int min, int max)
{
    unsigned int raw = GetNext(); // no cast needed
    int span = max - min + 1;
    return min + (raw % span);
}


float DrawScratchSpace::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

RGB DrawScratchSpace::AlphaBlend(const RGB& dst, const RGB& src)
{
    float a = src.a / 255.0f;       // incoming alpha
    float ia = 1.0f - a;            // inverse alpha

    RGB out;
    out.r = static_cast<uint8_t>(src.r * a + dst.r * ia);
    out.g = static_cast<uint8_t>(src.g * a + dst.g * ia);
    out.b = static_cast<uint8_t>(src.b * a + dst.b * ia);
    out.a = 255; // or keep dst.a, or compute new alpha — your choice

    return out;

}





RGB DrawScratchSpace::Lerp(RGB a, RGB b, float c)
{
    auto lerpChannel = [&](int A, int B) -> uint8_t
    {
        float v = A + (B - A) * c;

        // clamp to 0–255
        if (v < 0.0f) v = 0.0f;
        if (v > 255.0f) v = 255.0f;

        return static_cast<uint8_t>(v + 0.5f); // rounded
    };

    return {
        lerpChannel(a.r, b.r),
        lerpChannel(a.g, b.g),
        lerpChannel(a.b, b.b),
        lerpChannel(a.a, b.a)
    };
}
vec3d DrawScratchSpace::Lerp(vec3d a, vec3d b, float c)
{
    return a + (b - a) * c;
}
float DrawScratchSpace::Lerp(float a, float b, float c)
{
    return a + (b - a) * c;
}
int DrawScratchSpace::Lerp(int a, int b, int c)
{
    return a + (b - a) * c;
}
vec3d DrawScratchSpace::Arc(vec3d a, vec3d b, float h, float c)
{
    h = -h;
    // Base straight-line interpolation
    vec3d pos = Lerp(a, b, c);

    // Parabolic height factor (0 → 1 → 0)
    float arc = 4.0f * c * (1.0f - c);

    // Add height along Y axis
    pos.y += arc * h;

    return pos;
}

vec3d DrawScratchSpace::Arc(vec3d a, vec3d b, float h, float c, float direction)
{
    h = -h;
    // Base straight-line interpolation
    vec3d pos = Lerp(a, b, c);

    // Parabolic height factor (0 → 1 → 0)
    float arc = 4.0f * c * (1.0f - c);

    // Add height along Y axis
    pos.y += (arc * h)* direction;

    return pos;
}

vec3d DrawScratchSpace::ArcHorizontal(vec3d a, vec3d b, float h, float c, float direction)
{
    h = -h;
    // Base straight-line interpolation
    vec3d pos = Lerp(a, b, c);

    // Parabolic height factor (0 → 1 → 0)
    float arc = 4.0f * c * (1.0f - c);

    // Add height along Y axis
    pos.x += (arc * h)* direction;

    return pos;
}


Point DrawScratchSpace::RotatePoint(Point p, Point pivot, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    // Translate point to origin
    float x = p.x - pivot.x;
    float y = p.y - pivot.y;

    // Rotate
    float xNew = x * c - y * s;
    float yNew = x * s + y * c;

    // Translate back
    return {
        static_cast<int>(xNew + pivot.x),
        static_cast<int>(yNew + pivot.y)
    };
}

vec3d DrawScratchSpace::GetMovementInput()
{
    return vec3d{
        Input->GetMovementX(), 0, Input->GetMovementY()
    };
}



void DrawScratchSpace::MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m)
{
    o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
    o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
    o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

void DrawScratchSpace::MultiplyMatrixVectorArray(mat4x4& m, vec3d* in, vec3d* out, size_t count)
{
    //I think maybe possibly I can use this for parenting?
    //My idea is to pre-make all position and rotation vectors, and then rotate them all at once using the matrix rotation function (like the camera)
    //And I can do the same for sub objecs
    for (size_t i = 0; i < count; i++)
    {
        MultiplyMatrixVector(in[i], out[i], m);
    }
}

mat4x4 IdentityMatrix()
{
    mat4x4 m = {};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}
mat4x4 DrawScratchSpace::IdentityMatrix()
{
    mat4x4 m = {};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}
void DrawScratchSpace::SetCamera_Legacy(vec3d loc, vec3d target)
{
    CameraLoc = loc;
    CameraTargetLoc = target;

    //Draw camera: Draw3DLine(loc, target,RGB{255,255,255,255});

}
void DrawScratchSpace::SetCamera(vec3d loc, vec3d target)
{
    CameraLoc = loc;
    CameraTargetLoc = target - loc;

    //Draw camera: Draw3DLine(loc, target,RGB{255,255,255,255});

}

void DrawScratchSpace::SetCameraFOV(float nFov)
{
    // Projection Matrix
    float fNear = NearClip; //0.1f
    float fFar = 1000.0f;
    float fFov = nFov;
    float fAspectRatio = (float)SCREEN_Y / (float)SCREEN_X;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    MatrixProj.m[0][0] = fAspectRatio * fFovRad;
    MatrixProj.m[1][1] = fFovRad;
    MatrixProj.m[2][2] = fFar / (fFar - fNear);
    MatrixProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    MatrixProj.m[2][3] = 1.0f;
    MatrixProj.m[3][3] = 0.0f;
}

mat4x4 Matrix_MakeTranslation(float x, float y, float z)
{
    mat4x4 m = IdentityMatrix();
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
    return m;
}

mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
{
    mat4x4 matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] +
            m1.m[r][1] * m2.m[1][c] +
            m1.m[r][2] * m2.m[2][c] +
            m1.m[r][3] * m2.m[3][c];
    return matrix;
}


mat4x4 DrawScratchSpace::Matrix_QuickInverse(mat4x4& m)
{
    mat4x4 matrix = {};

    // Transpose rotation part
    matrix.m[0][0] = m.m[0][0];
    matrix.m[0][1] = m.m[1][0];
    matrix.m[0][2] = m.m[2][0];

    matrix.m[1][0] = m.m[0][1];
    matrix.m[1][1] = m.m[1][1];
    matrix.m[1][2] = m.m[2][1];

    matrix.m[2][0] = m.m[0][2];
    matrix.m[2][1] = m.m[1][2];
    matrix.m[2][2] = m.m[2][2];

    // Invert translation
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;

    return matrix;
}

float DotProduct(const vec3d& a, const vec3d& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
mat4x4 DrawScratchSpace::CalculateViewMatrix()
{
    // -----------------------------
 // Point the CAMERA
 // -----------------------------
    mat4x4 matView = IdentityMatrix();
    vec3d newForward = { 0,0,0 };
    vec3d up = { 0.0f, 1.0f, 0.0f };
    vec3d a = newForward * DotProduct(up, newForward);
    vec3d newUp = up - a;
    vec3d vCamera = CameraLoc;
    vec3d vLookDir = CameraTargetLoc;
    vec3d vTarget = vCamera + vLookDir;
    mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, { 2,1,0 });
    matView = Matrix_QuickInverse(matCamera);
    matView = Matrix_PointAt(vCamera, vCamera + vLookDir, up);
    matView = Matrix_QuickInverse(matView); //flip it upside down!
    // store camera-to-world for movement
    StoredCameraMatView = matView;
    return matView;
}
vec3d DrawScratchSpace::CrossProduct(const vec3d& a, const vec3d& b)
{
    vec3d r = { 0,0,0 };
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
float DrawScratchSpace::DotProduct(const vec3d& a, const vec3d& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
vec3d DrawScratchSpace::LookAtRotation(const vec3d& from, const vec3d& to)
{
    vec3d dir = to - from;

    //replace with normalize function?
    // Normalize direction
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (len > 0.0001f)
        dir = { dir.x / len, dir.y / len, dir.z / len };

    vec3d rot = { 0,0,0 };

    // YAW (rotation around Y axis)
    rot.y = atan2f(dir.x, dir.z);

    // PITCH (rotation around X axis)
    rot.x = -asinf(dir.y);

    // ROLL (no roll for now)
    rot.z = 0.0f;

    return rot;

}
vec3d DrawScratchSpace::LookAtRotation2D(const vec3d& from, const vec3d& to)
{
    return LookAtRotation(vec3d{from.x,from.y,from.z}, vec3d{to.x,from.y,to.z});
}
float DrawScratchSpace::Distance(const vec3d& a, const vec3d& b)
{
    vec3d d = b - a;
    return sqrtf(d.x*d.x + d.y*d.y + d.z*d.z);
}

float DrawScratchSpace::SquaredDistance(const vec3d & a, const vec3d & b)
{
    vec3d d = b - a;
    return (d.x * d.x + d.y * d.y + d.z * d.z);
}

float DrawScratchSpace::SquaredDistance2D(const vec3d& a, const vec3d& b)
{
    return SquaredDistance(vec3d{ a.x,0.0f,a.z }, vec3d{ b.x,0.0f,b.z });
}

bool DrawScratchSpace::PointInTriangle(const vec3d& p, const vec3d& a, const vec3d& b, const vec3d& c)
{
    vec3d v0 = b - a;
    vec3d v1 = c - a;
    vec3d v2 = p - a;

    float d00 = DotProduct(v0, v0);
    float d01 = DotProduct(v0, v1);
    float d11 = DotProduct(v1, v1);
    float d20 = DotProduct(v2, v0);
    float d21 = DotProduct(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    if (denom == 0.0f) return false; // degenerate triangle?

    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return (u >= 0.0f && v >= 0.0f && w >= 0.0f);

}

float DrawScratchSpace::SolveTriangleY(const vec3d& a, const vec3d& b, const vec3d& c, float px, float pz)
{
    vec3d n = CrossProduct(b - a, c - a);   // plane normal
    //https://tutorial.math.lamar.edu/classes/calcIII/EqnsOfPlanes.aspx
    // plane equation: n.x*x + n.y*y + n.z*z + d = 0  (right?)
    float d = -(n.x * a.x + n.y * a.y + n.z * a.z);

    // solve for y at (px, pz)
    return -(n.x * px + n.z * pz + d) / n.y;
}

float DrawScratchSpace::GroundHeightOnTriangle(const vec3d& a, const vec3d& b, const vec3d& c, float px, float pz)
{
    float py = SolveTriangleY(a,b, c, px, pz);
    vec3d p = { px, py, pz };

    if (PointInTriangle(p, a, b, c))
    {
        return py;
    }

    return NAN; // caller tries next triangle

}

vec3d DrawScratchSpace::SnapToMesh(const vec3d& worldPos, const Mesh& mesh, const vec3d& meshPos)
{
    //worldPos = player location in world space if snapping a player to the ground, for example
    // Convert world → mesh-local
    vec3d localPos = worldPos - meshPos; //Not working? (FIX IT HERE, and IN THE COLOR FUNCTION BELLOW)
    //subtracting meshPos 

    float bestY = NAN;
    int i = 0;
   
    for (const auto& tri : mesh.Tris)
    {
       
        vec3d a = tri.p[0] - meshPos;
        vec3d b = tri.p[1] - meshPos;
        vec3d c = tri.p[2] - meshPos;

        float y = GroundHeightOnTriangle(a, b, c, localPos.x, localPos.z);

        if (!std::isnan(y))
        {
            if (std::isnan(bestY) || y > bestY)
            {
                SnapToMeshTriColor = (tri.c[0]+ SnapToMeshTriColor = tri.c[1]+ SnapToMeshTriColor = tri.c[2]) /3;
                bestY = y;
            }
               
        }
      
        i++;
    }

    if (!std::isnan(bestY)) {
        LastSnapToMeshResult = true;
        
        return { worldPos.x, bestY + meshPos.y, worldPos.z };

    }
    
    LastSnapToMeshResult = false;
    return worldPos;



}

RGB DrawScratchSpace::GetColorOfClosestTri(const vec3d& worldPos, const Mesh& mesh, const vec3d& meshPos, RGB fallbackColor)
{
    //YOU CAN USE THIS TO CREATE LIGHT PROB MESH VOLUMES IN BLENDER! INFINIT CHEAP FAKE LIGHT! 
    //If you caclulate per triangle position on a mesh, and override that meshes vertex color on that triangle/vertex, you'll 
    // get 3D vertex light from a bunch of triangle "light probs" that are created in blender. Do you get it future me?
    // 
    //This is a copy of SnapToMesh, but witout the snapping. When I fix the world position issue in that function, I should fix it here!
    vec3d localPos = worldPos - meshPos; //Not working? (FIX IT HERE, and IN THE COLOR FUNCTION BELLOW)

    float bestY = NAN;
    int i = 0;
    for (const auto& tri : mesh.Tris)
    {

        vec3d a = tri.p[0] - meshPos;
        vec3d b = tri.p[1] - meshPos;
        vec3d c = tri.p[2] - meshPos;

        float y = GroundHeightOnTriangle(a, b, c, localPos.x, localPos.z);

        if (!std::isnan(y))
        {
            if (std::isnan(bestY) || y > bestY)
            {
                fallbackColor = (tri.c[0] + SnapToMeshTriColor = tri.c[1] + SnapToMeshTriColor = tri.c[2]) / 3;
                bestY = y;
            }

        }

        i++;
    }

    if (!std::isnan(bestY)) {

        return fallbackColor;
    }
    return fallbackColor;
}

triangle DrawScratchSpace::GetNearestTriangleInMeshRaw(const Mesh& m, vec3d p)
{
    //This is proably not a scalable solution. build out some kind of grid and use that to narrow down the triangles instead of this. 
    //This will get us up and running though.
    float closestDist = (float)1e+300;
    int triId = 0;
    for (int i = 0; i < m.Tris.size(); ++i)
    {
        vec3d averagePos = (m.Tris[i].p[0] + m.Tris[i].p[1] + m.Tris[i].p[2]) / 3;
        float curDist = SquaredDistance(averagePos, p);
        if (curDist < closestDist)
        {
            triId = i;
        }
    }
    return m.Tris[triId];
}

float DrawScratchSpace::Distance2D(const vec3d& a, const vec3d& b)
{
    return Distance(vec3d{a.x,0.0f,a.z}, vec3d{b.x,0.0f,b.z});
}
vec3d Normalize(const vec3d& v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f)
        return { 0, 0, 0 };

    return { v.x / length, v.y / length, v.z / length };
}

mat4x4 DrawScratchSpace::Matrix_PointAt(vec3d pos, vec3d target, vec3d up)
{
    // Calculate new forward direction
    vec3d newForward = target - pos;
    newForward = Normalize(newForward);

    // Calculate new up direction
    vec3d a = newForward * DotProduct(up, newForward);
    vec3d newUp = up - a;
    newUp = Normalize(newUp);

    // New right direction is cross product
    vec3d newRight = CrossProduct(newUp, newForward);

    // Construct dimensioning and translation matrix
    mat4x4 matrix = {};

    matrix.m[0][0] = newRight.x;
    matrix.m[0][1] = newRight.y;
    matrix.m[0][2] = newRight.z;

    matrix.m[1][0] = newUp.x;
    matrix.m[1][1] = newUp.y;
    matrix.m[1][2] = newUp.z;

    matrix.m[2][0] = newForward.x;
    matrix.m[2][1] = newForward.y;
    matrix.m[2][2] = newForward.z;

    matrix.m[3][0] = pos.x;
    matrix.m[3][1] = pos.y;
    matrix.m[3][2] = pos.z;
    matrix.m[3][3] = 1.0f;

    return matrix;
}
bool DrawScratchSpace::IsInView(const vec3d& c, float r,float NearClip, float FarClip,float tanHalfFovX, float tanHalfFovY)
{
    if (UseFrustumCulling == false)
    {
        return true; //lie to ourselves
    }
    // Behind camera entirely
    if (c.z + r < NearClip)
        return false;

    // Too far
    if (c.z - r > FarClip)
        return false;

    // Left/right
    if (fabs(c.x) > c.z * tanHalfFovX + r) //tangent half fov
        return false;

    // Top/bottom
    if (fabs(c.y) > c.z * tanHalfFovY + r) //tangent half fov
        return false;

    return true;
}
void DrawScratchSpace::DrawMesh(Mesh m)
{
    DrawMesh(m, { 0,0,0 }, { 1,0,0 });
}

void DrawScratchSpace::DrawMesh(Mesh m, vec3d loc)
{
    DrawMesh(m, { 0,0,0 }, { 1,0,0 });
}

void DrawScratchSpace::DrawMesh(Mesh m, vec3d loc, vec3d rot)
{
    DrawMesh(m, loc, rot, { 1,1,1 });
}
void DrawScratchSpace::DrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale)
{
    // -----------------------------
    // STORE DATA OTHER FUNCTIONS MAY NEED
    // -----------------------------
    LastLocation = loc;


    // -----------------------------
    // BUILD ROTATION MATRICES
    // -----------------------------
    mat4x4 matRotZ = {};
    mat4x4 matRotY = {};
    mat4x4 matRotX = {};

    // Rotation Z
    matRotZ.m[0][0] = cosf(rot.z);
    matRotZ.m[0][1] = sinf(rot.z);
    matRotZ.m[1][0] = -sinf(rot.z);
    matRotZ.m[1][1] = cosf(rot.z);
    matRotZ.m[2][2] = 1.0f;
    matRotZ.m[3][3] = 1.0f;

    // Rotation X
    matRotX.m[0][0] = 1.0f;
    matRotX.m[1][1] = cosf(rot.x);
    matRotX.m[1][2] = sinf(rot.x);
    matRotX.m[2][1] = -sinf(rot.x);
    matRotX.m[2][2] = cosf(rot.x);
    matRotX.m[3][3] = 1.0f;

   //Rotation Y
    // Rotation Y
    matRotY.m[0][0] = cosf(rot.y);
    matRotY.m[0][2] = -sinf(rot.y);
    matRotY.m[1][1] = 1.0f;
    matRotY.m[2][0] = sinf(rot.y);
    matRotY.m[2][2] = cosf(rot.y);
    matRotY.m[3][3] = 1.0f;


    // -----------------------------
    // BUILD TRANSLATION MATRIX
    // -----------------------------
    mat4x4 matTrans = IdentityMatrix();
    matTrans.m[3][0] = loc.x;
    matTrans.m[3][1] = loc.y;
    matTrans.m[3][2] = loc.z;
    // -----------------------------
    //SCALE MATRIX
    // -----------------------------
    mat4x4 matScale = {};
    matScale.m[0][0] = scale.x;
    matScale.m[1][1] = scale.y;
    matScale.m[2][2] = scale.z;
    matScale.m[3][3] = 1.0f;

    // -----------------------------
    // COMPOSE WORLD MATRIX
    // world = rotZ * rotX * translation
    // -----------------------------
    mat4x4 matWorld = Matrix_MultiplyMatrix(matRotZ, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matRotY);
    matWorld = Matrix_MultiplyMatrix(matWorld, matScale);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    // -----------------------------
    // CAMERA (still at origin for now)
    // view = identity (until you add a real camera)
    // -----------------------------
    mat4x4 matView = IdentityMatrix(); // camera view matrix
    // -----------------------------
    // Point the CAMERA
    // -----------------------------
    matView = CalculateViewMatrix();


    //------------------------------
    //FRUSTRUM CULLING
    //------------------------------
    vec3d centerWorld;
    vec3d centerMesh = vec3d{ 0,0,0 };
    MultiplyMatrixVector(centerMesh, centerWorld, matWorld);
    vec3d centerView;
    MultiplyMatrixVector(centerWorld, centerView, matView);
    float tanHalfFovX = 1.0f / MatrixProj.m[0][0];
    float tanHalfFovY = 1.0f / MatrixProj.m[1][1];
    //Make TangentHalfX/Y smaller for debugging
    if (!IsInView(centerView, m.cullingRadius, NearClip, FarClip,tanHalfFovX, tanHalfFovY))
    {
        DrawHighlightEdgeOnly = false; //turn this off in case we culled a special mesh
        //mesh not in camera view!
       // printf("MESH NOT IN VIEW!\n");
        return; 
    }



    //vec3d newForward;
    //vec3d up = { 0.0f, 1.0f, 0.0f };
    //vec3d a = newForward * DotProduct(up, newForward);
    //vec3d newUp = up - a;
    //vec3d vCamera = CameraLoc;
    //vec3d vLookDir = CameraTargetLoc;
    //vec3d vTarget = vCamera + vLookDir;
    //mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, { 2,1,0 });
    //matView = Matrix_QuickInverse(matCamera);
    //matView = Matrix_PointAt(vCamera, vCamera + vLookDir, up);
    //matView = Matrix_QuickInverse(matView); //flip it upside down!
    //StoredCameraMatView = matView;
//leaving the old camera stuff here but commented out. I like it in this function, and I thought I needed it in another, but it turns out I do not...
//Will decide if I want to keep  CalculateViewMatrix() or not soon...

    // -----------------------------
    //DRAW
    //-----------------------------
    std::vector<triangle> vecTrianglesToRaster;

    for (auto tri : m.Tris)
    {
       

        triangle triWorld, triViewed, triProjected;

        // Apply WORLD transform
        MultiplyMatrixVector(tri.p[0], triWorld.p[0], matWorld);
        MultiplyMatrixVector(tri.p[1], triWorld.p[1], matWorld);
        MultiplyMatrixVector(tri.p[2], triWorld.p[2], matWorld);

        // Apply VIEW transform (identity for now)
        MultiplyMatrixVector(triWorld.p[0], triViewed.p[0], matView);
        MultiplyMatrixVector(triWorld.p[1], triViewed.p[1], matView);
        MultiplyMatrixVector(triWorld.p[2], triViewed.p[2], matView);

        // Back-face cull
        vec3d line1 = triViewed.p[1] - triViewed.p[0];
        vec3d line2 = triViewed.p[2] - triViewed.p[0];
        vec3d normal = Normalize(CrossProduct(line1, line2));
        if (DotProduct(normal, triViewed.p[0]) >= 0.0f)
            continue;

        //OLD BUT GOOD CULLING 02-15-2026//OLD CULLING 02-15-2026//OLD CULLING 02-15-2026
// Near-plane cull (THROW THE WHOLE TRIANGLE AWAY! SUPER AGRESSIVE, FASTER)
       if (triViewed.p[0].z <= NearClip ||
            triViewed.p[1].z <= NearClip ||
            triViewed.p[2].z <= NearClip)
            continue;
//OLD CULLING 02-15-2026//OLD CULLING 02-15-2026//OLD CULLING 02-15-2026

//NEW CULLING SHOULD LET ME HAVE A LOWER POLYGON WORLD IN THEORY. NEEDS TESTING
    // Skip triangles with ANY vertex behind the camera
        //if (triViewed.p[0].z <= 0.0f ||
        //    triViewed.p[1].z <= 0.0f ||
        //    triViewed.p[2].z <= 0.0f)
        //    continue;

        //// Skip triangles where ALL vertices are behind the near plane
        //if (triViewed.p[0].z <= NearClip &&
        //    triViewed.p[1].z <= NearClip &&
        //    triViewed.p[2].z <= NearClip)
        //    continue;
 //NEW CULLING SHOULD LET ME HAVE A LOWER POLYGON WORLD IN THEORY. NEEDS TESTING





        // Apply PROJECTION transform
        MultiplyMatrixVector(triViewed.p[0], triProjected.p[0], MatrixProj);
        MultiplyMatrixVector(triViewed.p[1], triProjected.p[1], MatrixProj);
        MultiplyMatrixVector(triViewed.p[2], triProjected.p[2], MatrixProj);


       


        // Scale into view space
        triProjected.p[0].x = (triProjected.p[0].x + 1.0f) * 0.5f * SCREEN_X;
        triProjected.p[0].y = (triProjected.p[0].y + 1.0f) * 0.5f * SCREEN_Y;

        triProjected.p[1].x = (triProjected.p[1].x + 1.0f) * 0.5f * SCREEN_X;
        triProjected.p[1].y = (triProjected.p[1].y + 1.0f) * 0.5f * SCREEN_Y;

        triProjected.p[2].x = (triProjected.p[2].x + 1.0f) * 0.5f * SCREEN_X;
        triProjected.p[2].y = (triProjected.p[2].y + 1.0f) * 0.5f * SCREEN_Y;

        //Store Z Info 
        triProjected.depth = (triViewed.p[0].z + triViewed.p[1].z + triViewed.p[2].z) / 3.0f;

        //copy color
        triProjected.c[0] = tri.c[0];
        triProjected.c[1] = tri.c[1];
        triProjected.c[2] = tri.c[2];

        //copy uvs
        triProjected.uv[0] = tri.uv[0];
        triProjected.uv[1] = tri.uv[1];
        triProjected.uv[2] = tri.uv[2];
       //copy normals
        // After WORLD transform:

        vec3d n = ComputeTriangleNormal(triWorld.p[0], triWorld.p[1], triWorld.p[2]);
        //n = ComputeTriangleNormal(triViewed.p[0], triViewed.p[1], triViewed.p[2]);
        //TODO PUT [3] NORMALS, ONE PER VERTEX IN TO THE TRIANGLE FOR SMOOTH SHADING INSTEAD OF THIS


        // Bias toward camera direction hack:
        //vec3d viewDir = {-matView.m[0][2],-matView.m[1][2],-matView.m[2][2]}; // attempt to extract camera direction...
        //n = Normalize(n * 0.7f + viewDir * 0.3f);



        triProjected.normal[0] = Normalize(n); //use n for "real" normals
        triProjected.normal[1] = Normalize(n);
        triProjected.normal[2] = Normalize(n);  //This all needs to be pulled out of here, and it needs to happen on mesh load I think : ( 



       




        vecTrianglesToRaster.push_back(triProjected);



    }


   


    // Painter’s sort
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(),
        [](triangle& t1, triangle& t2)
    {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });
    float ZFog = 0.0f;
    // Rasterize
    for (auto& triProjected : vecTrianglesToRaster)
    {
        //dumb fog / bad fog /bs shading
       
        if (!DrawUnlit)
        {
            if (UseFogHackyShading)
            {
                ZFog += 4.0f / vecTrianglesToRaster.size() * 0.2f;  //<-Old lighting model
               
               
            }
            else
            {
                ZFog = 1.0f;
            }
        }
        else
        {

            ZFog = 1.0f;
        }
        //todo: Add a shading mode enum.
        //Add Gouraud Shading, but keep dumb fog as an option, since it's interesting...
        //also add flat shading and a light source option...
        //https://www.gabrielgambetta.com/computer-graphics-from-scratch/13-shading.html
        //https://www.gabrielgambetta.com/computer-graphics-from-scratch/03-light.html

        //STANDARD RAINBOW COLOR TRIS
       /* Vertex p0 = { triProjected.p[0].x, triProjected.p[0].y, {255 * ZFog,0,0,255} };
        Vertex p1 = { triProjected.p[1].x, triProjected.p[1].y, {0,255 * ZFog,0,255} };
        Vertex p2 = { triProjected.p[2].x, triProjected.p[2].y, {0,0,255 * ZFog,255} };*/

        //CRAZY
        /*Vertex p0 = { triProjected.p[0].x, triProjected.p[0].y, {ZFog,255,255,255} };
        Vertex p1 = { triProjected.p[1].x, triProjected.p[1].y, {255, ZFog,255,255} };
        Vertex p2 = { triProjected.p[2].x, triProjected.p[2].y, {255, 255, ZFog,255} };*/


        //Rainbow tinted + fog (divided by 75% to bring down brightness)
        int R = MeshColor.r * ZFog * 0.75f;
        int G = MeshColor.g * ZFog * 0.75f;
        int B = MeshColor.b * ZFog * 0.75f;

        if (UseDepthFog)
        {
            //these numbers are insane. I may need to find the max depth of the color depth, and divide by that instead. You can track when you write to the buffer what that value is!
            //reset on buffer clear
            R = R - (triProjected.depth);
            G = G - (triProjected.depth);
            B = B - (triProjected.depth);
        }
        

        
    


        //----------------------------------------------------------------------------------------------------------------------
        //COLOR FROM F+GLOBAL MESH COLOR:
        //----------------------------------------------------------------------------------------------------------------------
        Vertex p0 = { static_cast<int>(triProjected.p[0].x), static_cast<int>(triProjected.p[0].y), RGB{1*R,G,B}};
        Vertex p1 = { static_cast<int>(triProjected.p[1].x), static_cast<int>(triProjected.p[1].y), RGB{R,1*G,B}};
        Vertex p2 = { static_cast<int>(triProjected.p[2].x), static_cast<int>(triProjected.p[2].y), RGB{R,G,1*B}};
        

        //lighting
        if (UseGouraudShading && !DrawUnlit)
        {
            RGB color0 = GouraudShade(triProjected.normal[0], p0.color);
            RGB color1 = GouraudShade(triProjected.normal[1], p1.color);
            RGB color2 = GouraudShade(triProjected.normal[2], p2.color);
            p0.color = color0;
            p1.color = color1;
            p2.color = color2;
        }
       


     //VERTEX COLOR:::::::::::::::::
        ////FORVE VERTEX COLORS ONLY
        //p0.color = triProjected.c[0];
        //p1.color = triProjected.c[1];
        //p2.color = triProjected.c[2];


        //TINT WITH VERTEX COLORS
        p0.color.r = (p0.color.r * triProjected.c[0].r) / 255;
        p0.color.g = (p0.color.g * triProjected.c[0].g) / 255;
        p0.color.b = (p0.color.b * triProjected.c[0].b) / 255;


        p1.color.r = (p1.color.r * triProjected.c[1].r) / 255;
        p1.color.g = (p1.color.g * triProjected.c[1].g) / 255;
        p1.color.b = (p1.color.b * triProjected.c[1].b) / 255;


        p2.color.r = (p2.color.r * triProjected.c[2].r) / 255;
        p2.color.g = (p2.color.g * triProjected.c[2].g) / 255;
        p2.color.b = (p2.color.b * triProjected.c[2].b) / 255;

       


       /* p0.color = { 255,255,255 };
        p1.color = { 255,255,255 };
        p2.color = { 255,255,255 };*/
    //FORCE VERTEX COLOR:::::::::::::::::


        //----------------------------------------------------------------------------------------------------------------------
        //TEXTURING
        //---------------------------------------------------------------------------------------------------------------------- 
      
        
       
        p0.u = triProjected.uv[0].u + UvOffsetGlobal.u;
        p0.v = triProjected.uv[0].v + UvOffsetGlobal.v;
        p1.u = triProjected.uv[1].u + UvOffsetGlobal.u;
        p1.v = triProjected.uv[1].v + UvOffsetGlobal.v;
        p2.u = triProjected.uv[2].u + UvOffsetGlobal.u;
        p2.v = triProjected.uv[2].v + UvOffsetGlobal.v;

      
       
        //----------------------------------------------------------------------------------------------------------------------


        ////LIGHT FX (Won't work since you draw on top of tris that are already connected to the next one...
        //Either do the stamp buffer, or collect all tris and draw at the end in 2 passes...
        /*Vertex light_fx_p0 = { triProjected.p[0].x, triProjected.p[0].y, {255,255,255,122} };
        Vertex light_fx_p1 = { triProjected.p[1].x, triProjected.p[1].y, {255,255,255,122} };
        Vertex light_fx_p2 = { triProjected.p[2].x, triProjected.p[2].y, {255,255,255,122} };
        DrawTriangle(light_fx_p0 -1, light_fx_p1-1, light_fx_p2-0);*/

        //The outline is triangles that connect to a front facing triangle and a backfacing one I think...
        //use that info for outline only drawing in the future.
        //only do upper left, and left side for more style...


        
        
        int DepthValue = (int)((((triProjected.depth) * 0.5f) * 64) - ZOffset);
        //NORMAL DRAW or Highlight offset
        if (DrawHighlightEdgeOnly)
        {


            Vertex light_fx_p0 = { static_cast<int>(triProjected.p[0].x), static_cast<int>(triProjected.p[0].y), p0.color * HighlightBrightness };
            Vertex light_fx_p1 = { static_cast<int>(triProjected.p[1].x), static_cast<int>(triProjected.p[1].y),p1.color *HighlightBrightness };
            Vertex light_fx_p2 = { static_cast<int>(triProjected.p[2].x), static_cast<int>(triProjected.p[2].y), p2.color  *HighlightBrightness };

            DrawTriangle(light_fx_p0 - 1, light_fx_p1 - 1, light_fx_p2 - 1, DepthValue);
        }
        else
        {
            //Normal
            DrawTriangle(p0, p1, p2, DepthValue);
        }
        

        //DRAW VERTICIES
        if (DrawVerticies) {
            DrawSquare(p0.x, p0.y, 1, { 255,0,0,128 });
            DrawSquare(p1.x, p1.y, 2, { 0,255,0,128 });
            DrawSquare(p2.x, p2.y, 3, { 0,0,255,128 });
        }
        if (DrawEdges)
        {
            DrawLine(p0.x, p0.y, p1.x, p1.y, { EdgeBrightness,EdgeBrightness,0,255 });
            DrawLine(p1.x, p1.y, p2.x, p2.y, { EdgeBrightness,0,EdgeBrightness ,255 });
            DrawLine(p2.x, p2.y, p0.x, p0.y, { 0,EdgeBrightness,EdgeBrightness,255 });
        }
      

        
    }
    DrawHighlightEdgeOnly = false;
}

void DrawScratchSpace::DrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale, bool edge_light)
{
    DrawMesh(m, loc, rot, scale, edge_light, true);
}

void DrawScratchSpace::DrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale, bool edge_light, bool texture_edge_light)
{
    if (edge_light)
    {
        bool remember_light_setting = DrawUnlit;
        bool RememberTextureDrawOn = TextureDrawOn;
        RGB StoreColor = MeshColor;
        //multipass
        DrawHighlightEdgeOnly = true; //this causes the offset
        //const float offset = -10.5f;
        //const int offset = 215;
        const int offset = 64;
        ZOffset -= offset; // draw one point behind the mesh
        DrawUnlit = true;

        if (texture_edge_light == false)
        {
            MeshColor = MeshColor * 1.15f;
            TextureDrawOn = false;

        }
        
        //DRAW 1
        DrawMesh(m, loc, rot, scale);

        if (texture_edge_light == false)
        {
            TextureDrawOn = RememberTextureDrawOn;
        }
        
        MeshColor = StoreColor;
        ZOffset += offset; //restore mesh back to normal depth location
        DrawUnlit = remember_light_setting;

        //DRAW 2
        DrawMesh(m, loc, rot, scale);
    }
    else
    {
        DrawMesh(m, loc, rot, scale);
    }
}


void DrawScratchSpace::DifferDrawMesh(Mesh m)
{
    recipes.push_back({ m, vec3d{1,1,1}, vec3d{1,0,0}, vec3d{1,1,1}, false,MeshColor });
}

void DrawScratchSpace::DifferDrawMesh(Mesh m, vec3d loc)
{
    recipes.push_back({ m, loc, vec3d{1,0,0}, vec3d{1,1,1}, false,MeshColor });
}

void DrawScratchSpace::DifferDrawMesh(Mesh m, vec3d loc, vec3d rot)
{
    recipes.push_back({ m, loc, rot, vec3d{1,1,1}, false,MeshColor });
}

void DrawScratchSpace::DifferDrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale)
{
    recipes.push_back({ m, loc, rot, scale, false,MeshColor });
}

void DrawScratchSpace::DifferDrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale, bool edge_light)
{
    recipes.push_back({ m, loc, rot, scale, edge_light,MeshColor });
}

void DrawScratchSpace::DrawSortedDifferedMeshes()
{
    //Set ZWriteOn to false to get old 2D sorting feature! :: MyScratch->ZWriteOn = false;

    std::sort(recipes.begin(), recipes.end(),
        [&](const SortedMeshRecipe& a, const SortedMeshRecipe& b)
    {
        double da = (a.loc - CameraLoc).length_squared();
        double db = (b.loc - CameraLoc).length_squared();
        return da > db;   // nearest first
    });

    for (int i = 0; i < recipes.size(); ++i)
    {
        MeshColor = recipes[i].MeshColor;
        DrawMesh(recipes[i].m, recipes[i].loc, recipes[i].rot, recipes[i].scale ,recipes[i].edge_light );
    }
    recipes.clear();
}

vec3d DrawScratchSpace::ComputeTriangleNormal(const vec3d& p0, const vec3d& p1, const vec3d& p2)
{
    vec3d a = p1 - p0;
    vec3d b = p2 - p0;
    return Normalize(CrossProduct(a, b));
}
RGB DrawScratchSpace::GouraudShade(const vec3d& normal, const RGB& base)
{
    //Search Helper: Lambert Phong Shading Lighting 
    // 
    // 
    // Simple directional light
    vec3d lightDir = Normalize(LightDir);

    // Lambert term
    //float ndotl = std::max(0.0f, DotProduct(normal, lightDir));

    //Half Lamber term instead
    float ndotl = DotProduct(normal, lightDir);
    //ndotl = ndotl * 0.5f + 0.5f;   //using Half-Lambert instead...changed my mind. 
    ndotl = Clamp(ndotl, 0.25f, 1.0f); //changed clamp so it can't go super dark. Better results then the half lambert?

    // Apply to color
    RGB out;
    out.r = (int)(base.r * ndotl);
    out.g = (int)(base.g * ndotl);
    out.b = (int)(base.b * ndotl);
    out.a = base.a;

    return out;
}
void DrawScratchSpace::DrawSprite3D(Sprite s, vec3d loc, vec3d rot, vec3d scale)
{
    // Build WORLD matrix (Scale → RotZ → RotX → Trans)
    mat4x4 matScale = {};
    matScale.m[0][0] = scale.x;
    matScale.m[1][1] = scale.y;
    matScale.m[2][2] = scale.z;
    matScale.m[3][3] = 1.0f;

    mat4x4 matRotZ = {};
    matRotZ.m[0][0] = cosf(rot.z);
    matRotZ.m[0][1] = sinf(rot.z);
    matRotZ.m[1][0] = -sinf(rot.z);
    matRotZ.m[1][1] = cosf(rot.z);
    matRotZ.m[2][2] = 1.0f;
    matRotZ.m[3][3] = 1.0f;

    mat4x4 matRotX = {};
    matRotX.m[0][0] = 1.0f;
    matRotX.m[1][1] = cosf(rot.x);
    matRotX.m[1][2] = sinf(rot.x);
    matRotX.m[2][1] = -sinf(rot.x);
    matRotX.m[2][2] = cosf(rot.x);
    matRotX.m[3][3] = 1.0f;

    mat4x4 matTrans = IdentityMatrix();
    matTrans.m[3][0] = loc.x;
    matTrans.m[3][1] = loc.y;
    matTrans.m[3][2] = loc.z;

    mat4x4 matWorld = Matrix_MultiplyMatrix(matScale, matRotZ);
    matWorld = Matrix_MultiplyMatrix(matWorld, matRotX);
    matWorld = Matrix_MultiplyMatrix(matWorld, matTrans);

    // CAMERA VIEW MATRIX
    vec3d up = { 0,1,0 };
    vec3d vCamera = CameraLoc;
    vec3d vLookDir = CameraTargetLoc;
    vec3d vTarget = vCamera + vLookDir;

    mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, up);
    mat4x4 matView = Matrix_QuickInverse(matCamera);

    // ------------------------------------
    // Transform sprite origin (0,0,0)
    // ------------------------------------
    vec3d pWorld, pView, pProj;

    vec3d origin = { 0,0,0 };

    MultiplyMatrixVector(origin, pWorld, matWorld);
    MultiplyMatrixVector(pWorld, pView, matView);

    // Cull if behind camera
    if (pView.z <= NearClip)
        return;

    // Project
    MultiplyMatrixVector(pView, pProj, MatrixProj);

    // Convert to screen space
    int spriteX = (pProj.x + 1.0f) * 0.5f * SCREEN_X;
    int spriteY = (pProj.y + 1.0f) * 0.5f * SCREEN_Y;

    // Draw the sprite
    DrawSprite(spriteX, spriteY, s.pixels, s.width,s.height,false);
}

Mesh DrawScratchSpace::MorphMesh(const Mesh& from, const Mesh& to, float t)
{
    Mesh out;
    out.Tris.resize(from.Tris.size());

    for (size_t i = 0; i < from.Tris.size(); i++)
    {
        const triangle& A = from.Tris[i];
        const triangle& B = to.Tris[i];
        triangle& R = out.Tris[i];

      
        for (size_t j = 0; j < 3; j++)
        {
            R.p[j].x = Lerp(A.p[j].x, B.p[j].x, t);
            R.p[j].y = Lerp(A.p[j].y, B.p[j].y, t);
            R.p[j].z = Lerp(A.p[j].z, B.p[j].z, t);


            R.uv[j] = A.uv[j]; //You can have morphing uvs if you tween these!
            R.c[j] = A.c[j];  //IDEA! You can have morphing vertex colors if you tween these!
            
        }
    }
    return out;
}

//Mesh DrawScratchSpace::WaveMesh(const Mesh& from, float time, float strength)
//{
//    Mesh out;
//    out.Tris.resize(from.Tris.size());
//
//    for (size_t i = 0; i < from.Tris.size(); i++)
//    {
//        const triangle& A = from.Tris[i];
//        triangle& R = out.Tris[i];
//
//        for (size_t j = 0; j < 3; j++)
//        {
//            R.p[j].x = A.p[j].x + (sin(time + i) * strength);
//            R.p[j].y = A.p[j].y + (cos(time + i) * strength);
//            R.p[j].z = A.p[j].z;// +(sin(time + i) * strength);
//        }
//    }
//    return out;
//}
Mesh DrawScratchSpace::WaveMesh(const Mesh& from, float time, float strength)
{
    Mesh out;
    out.Tris.resize(from.Tris.size());

    for (size_t i = 0; i < from.Tris.size(); i++)
    {
        const triangle& A = from.Tris[i];
        triangle& R = out.Tris[i];

        R = A; // copy ONCE

        for (size_t j = 0; j < 3; j++)
        {
            float wave =
                sin(time + A.p[j].x * 0.1f + A.p[j].z * 0.1f) * strength;

            R.p[j].y = A.p[j].y + wave;
        }
    }

    return out;
}

vec3d DrawScratchSpace::Get2DPointInFromSpace(vec3d loc)
{
    mat4x4 matTrans = IdentityMatrix();
    matTrans.m[3][0] = loc.x;
    matTrans.m[3][1] = loc.y;
    matTrans.m[3][2] = loc.z;
    mat4x4 matWorld  = matTrans;
    vec3d origin = { 0,0,0 };


    // CAMERA VIEW MATRIX
    vec3d up = { 0,1,0 };
    vec3d vCamera = CameraLoc;
    vec3d vLookDir = CameraTargetLoc;
    vec3d vTarget = vCamera + vLookDir;

    mat4x4 matCamera = Matrix_PointAt(vCamera, vTarget, up);
    mat4x4 matView = Matrix_QuickInverse(matCamera);
    vec3d pWorld, pView, pProj;
    MultiplyMatrixVector(origin, pWorld, matWorld);
    MultiplyMatrixVector(pWorld, pView, matView);



    // Project
    MultiplyMatrixVector(pView, pProj, MatrixProj);

    int X = (pProj.x + 1.0f) * 0.5f * SCREEN_X;
    int Y = (pProj.y + 1.0f) * 0.5f * SCREEN_Y;


    // Cull if behind camera
    if (pView.z <= NearClip)
        return vec3d{ -32, -32, 0 };



    return vec3d{ (float)X, (float)Y, 0 };
}

vec3d DrawScratchSpace::Get2DPointFromLastLocation()
{
   return Get2DPointInFromSpace(LastLocation);
}


vec3d DrawScratchSpace::Normalize(vec3d input)
{
    float len = std::sqrt(input.x * input.x + input.y * input.y + input.z * input.z);
    if (len == 0.0f)
    {
        return vec3d{ 0.0f, 0.0f, 0.0f };
    }
    float inv = 1.0f / len;
    return vec3d
    {
        input.x * inv,
        input.y * inv,
        input.z * inv 
    };
}

void DrawScratchSpace::SetFade(RGB color, float a)
{
    if (a < 0.0f) { a = 0.0f; }
    if (a > 1.0f) { a = 1.0f; }
    int alpha = (int)(a * 255);
    
    color.a = alpha;
    DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, color);
}

void DrawScratchSpace::SetFade(RGB topLeft, RGB topRight, RGB bottomLeft, RGB bottomRight, float a)
{
    if (a < 0.0f) { a = 0.0f; }
    if (a > 1.0f) { a = 1.0f; }
    int alpha = (int)(a * 255);
    topLeft.a = alpha;
    topRight.a = alpha;
    bottomLeft.a = alpha;
    bottomRight.a = alpha;
    DrawRectangle(0, 0, SCREEN_X, SCREEN_Y, topLeft, topRight, bottomLeft, bottomRight);
}

DrawScratchSpace::~DrawScratchSpace()
{
    delete Input;
    Input = nullptr;
}
