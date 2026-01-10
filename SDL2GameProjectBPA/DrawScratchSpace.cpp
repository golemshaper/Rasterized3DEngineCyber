#include "DrawScratchSpace.h"
#include <random>
#include <algorithm>
#include "TextSprites.h"

void DrawScratchSpace::MoveMainspaceToExtraBuffer() 
{
    //Take whatever is on the screen, and back it up in the extra buffer so you can clear it and draw new stuff to add or multiply later
    std::copy(std::begin(MainSpace), std::end(MainSpace), ExtraBuffer);
}

void DrawScratchSpace::MultiplyBuffers() 
{
    for (int i = 0; i < TOTAL_PIXELS; ++i) 
    {
        MainSpace[i] = MainSpace[i] * ExtraBuffer[i];
    }
}

void DrawScratchSpace::AddBuffers()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = MainSpace[i] + ExtraBuffer[i];
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
        ZBuffer[i] = RGB{ 1024, 1024, 1024 ,1024 };
    }
}

void DrawScratchSpace::DrawZBufffer()
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        MainSpace[i] = RGB{255,255,255,255} - ZBuffer[i];
    }
}

void DrawScratchSpace::PushBackDepthBuffer(int amount)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {
        ZBuffer[i] = ZBuffer[i] + RGB{ amount,amount,amount,amount };
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

void DrawScratchSpace::CopyBufferToBuffer(RGB* from, RGB* to)
{
    for (int i = 0; i < TOTAL_PIXELS; ++i)
    {

        to[i] = from[i];
    }
}

/// <summary>
/// Only call me once. Use Clear() after if you want to clear the screen buffer
/// </summary>
/// <param name="wipe"></param>
void DrawScratchSpace::Initialize()
{
    Initialize(RGB{ 0,0,0 });
    Input = new InputWraper();
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
    float fNear = 0.1f;
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

void DrawScratchSpace::RandomScreenFill()
{
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
            if (ZWriteOn && ZBuffer[y * SCREEN_X + x].r < z)
            {
                //don't draw hidden triangle parts!
                continue;
            }
            if (ZWriteOn)
            {
                ZBuffer[y * SCREEN_X + x] = RGB{ z,z,z,255 };
            }
            MainSpace[y * SCREEN_X + x] = color;
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
            MainSpace[y * SCREEN_X + x] = color;
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
            out[i].a = 255;
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

            MainSpace[screenIndex] = pixel;
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
void DrawScratchSpace::DrawText(int X, int Y, RGB color, const char* text, TextSprites* tSprites)
{
    DrawText(X,Y,color,text,tSprites,1.0f);
}

void DrawScratchSpace::DrawText(int X, int Y, RGB color, const char* text, TextSprites* tSprites, float amount_revealed)
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

int DrawScratchSpace::GetRandom(int a, int b)
{
    /*std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);*/
    return GetNext(a, b);
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
void DrawScratchSpace::SetCamera(vec3d loc, vec3d target)
{
    CameraLoc = loc;
    CameraTargetLoc = target;
}
void DrawScratchSpace::SetCameraFOV(float nFov)
{
    // Projection Matrix
    float fNear = 0.1f;
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
    vec3d newForward;
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
    vec3d r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}
vec3d DrawScratchSpace::LookAtRotation(const vec3d& from, const vec3d& to)
{
    vec3d dir = to - from;

    //replace with normalize function?
    // Normalize direction
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if (len > 0.0001f)
        dir = { dir.x / len, dir.y / len, dir.z / len };

    vec3d rot;

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

        // Near-plane cull
        if (triViewed.p[0].z <= 0.1f ||
            triViewed.p[1].z <= 0.1f ||
            triViewed.p[2].z <= 0.1f)
            continue;

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
        //dumb fog
        ZFog += 4.0f/vecTrianglesToRaster.size() * 0.2f;

        //STANDARD RAINBOW COLOR TRIS
       /* Vertex p0 = { triProjected.p[0].x, triProjected.p[0].y, {255 * ZFog,0,0,255} };
        Vertex p1 = { triProjected.p[1].x, triProjected.p[1].y, {0,255 * ZFog,0,255} };
        Vertex p2 = { triProjected.p[2].x, triProjected.p[2].y, {0,0,255 * ZFog,255} };*/

        //CRAZY
        /*Vertex p0 = { triProjected.p[0].x, triProjected.p[0].y, {ZFog,255,255,255} };
        Vertex p1 = { triProjected.p[1].x, triProjected.p[1].y, {255, ZFog,255,255} };
        Vertex p2 = { triProjected.p[2].x, triProjected.p[2].y, {255, 255, ZFog,255} };*/


        //Rainbow tinted + fog (divided by 2 to bring down brightness)
        int R = MeshColor.r * ZFog * 0.75f;
        int G = MeshColor.g * ZFog * 0.75f;
        int B = MeshColor.b * ZFog * 0.75f;

        //COLOR FROM F+GLOBAL MESH COLOR:
        Vertex p0 = { static_cast<int>(triProjected.p[0].x), static_cast<int>(triProjected.p[0].y), {1*R,G,B} };
        Vertex p1 = { static_cast<int>(triProjected.p[1].x), static_cast<int>(triProjected.p[1].y), {R,1*G,B} };
        Vertex p2 = { static_cast<int>(triProjected.p[2].x), static_cast<int>(triProjected.p[2].y), {R,G,1*B} };


        ////LIGHT FX (Won't work since you draw on top of tris that are already connected to the next one...
        //Either do the stamp buffer, or collect all tris and draw at the end in 2 passes...
        /*Vertex light_fx_p0 = { triProjected.p[0].x, triProjected.p[0].y, {255,255,255,122} };
        Vertex light_fx_p1 = { triProjected.p[1].x, triProjected.p[1].y, {255,255,255,122} };
        Vertex light_fx_p2 = { triProjected.p[2].x, triProjected.p[2].y, {255,255,255,122} };
        DrawTriangle(light_fx_p0 -1, light_fx_p1-1, light_fx_p2-0);*/

        //The outline is triangles that connect to a front facing triangle and a backfacing one I think...
        //use that info for outline only drawing in the future.
        //only do upper left, and left side for more style...


        //Draw to ZBuffer (EXPERIMENTAL)
        
        //NOTE: IF Zoffset being an int is too imprecise, we can do the same thing using a float, but added to the tris depth!
        //We probably should do it this way for the precision! But for right now, I will not
        //DrawTriangleToZBuffer(p0, p1, p2, (int)((((triProjected.depth)*0.5f) * 64)-ZOffset));
        int DepthValue = (int)((((triProjected.depth) * 0.5f) * 64) - ZOffset);
        //NORMAL DRAW or Highlight offset
        if (DrawHighlightEdgeOnly)
        {
            //Highlight the Game needs to turn this on, and then draw two mesh passes (It auto-turns off after the mesh is done rendering.
            Vertex light_fx_p0 = { static_cast<int>(triProjected.p[0].x), static_cast<int>(triProjected.p[0].y), RGB{R,G,B}*2 };
            Vertex light_fx_p1 = { static_cast<int>(triProjected.p[1].x), static_cast<int>(triProjected.p[1].y), RGB{R,G,B}*2 };
            Vertex light_fx_p2 = { static_cast<int>(triProjected.p[2].x), static_cast<int>(triProjected.p[2].y), RGB{R,G,B}*2 };
            DrawTriangle(light_fx_p0 - 2, light_fx_p1 - 1, light_fx_p2 - 1, DepthValue);
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
    if (edge_light)
    {
        //multipass
        DrawHighlightEdgeOnly = true;
        //const float offset = -10.5f;
        const int offset = 15;
        ZOffset -= offset; // draw one point behind the mesh
        DrawMesh(m, loc, rot, scale);
        ZOffset += offset; //restore mesh back to normal depth location
        DrawMesh(m, loc, rot, scale);
    }
    else
    {
        DrawMesh(m,loc,rot,scale);
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
    if (pView.z <= 0.1f)
        return;

    // Project
    MultiplyMatrixVector(pView, pProj, MatrixProj);

    // Convert to screen space
    int spriteX = (pProj.x + 1.0f) * 0.5f * SCREEN_X;
    int spriteY = (pProj.y + 1.0f) * 0.5f * SCREEN_Y;

    // Draw the sprite
    DrawSprite(spriteX, spriteY, s);
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
    if (pView.z <= 0.1f)
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
