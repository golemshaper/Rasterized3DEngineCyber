#include "DrawScratchSpace.h"
#include <random>
#include <algorithm>

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

/// <summary>
/// Only call me once. Use Clear() after if you want to clear the screen buffer
/// </summary>
/// <param name="wipe"></param>
void DrawScratchSpace::Initialize()
{
    Initialize(RGB{ 0,0,0 });
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

void DrawScratchSpace::DrawSquare(int x, int y, int size, RGB color) {
    for (int dy = 0; dy < size; ++dy) {
        for (int dx = 0; dx < size; ++dx) {
            int px = x + dx;
            int py = y + dy;

            // Bounds check
            if (px >= 0 && px < SCREEN_X && py >= 0 && py < SCREEN_Y) {
                int index = py * SCREEN_X + px;
                MainSpace[index] = color;
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

            MainSpace[y * SCREEN_X + x] = color;
        }
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
void DrawScratchSpace::DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight, float angle)
{
    RGB Black = { 0, 0, 0 };
    Point pivot = { spriteWidth / 2.0f, spriteHeight / 2.0f };

    for (int y = 0; y < spriteHeight; ++y) {
        for (int x = 0; x < spriteWidth; ++x) {
            Point local = { static_cast<float>(x), static_cast<float>(y) };
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
            MainSpace[index] = (MainSpace[index] + color) /2;
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

float DrawScratchSpace::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
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
mat4x4 DrawScratchSpace::IdentityMatrix()
{
    mat4x4 m = {};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}
void DrawScratchSpace::DrawMesh(Mesh m, float DeltaTime)
{
    DrawMesh(m, { 0,0,0 }, DeltaTime);
}
void DrawScratchSpace::DrawMesh(Mesh m,vec3d loc,float DeltaTime)
{
    //for now we will pass in DeltaTime for testing the roation.
    //I'll do that some place else in the furure.
    //UPDATE THIS IN THE GAME TICK, OR IT'll GET FASTER FOR EACH MESH!    fTheta += 12.0f * DeltaTime;
    // Set up rotation matrices
    mat4x4 matRotZ, matRotX;
    

    // Rotation Z
    matRotZ.m[0][0] = cosf(fTheta);
    matRotZ.m[0][1] = sinf(fTheta);
    matRotZ.m[1][0] = -sinf(fTheta);
    matRotZ.m[1][1] = cosf(fTheta);
    matRotZ.m[2][2] = 1;
    matRotZ.m[3][3] = 1;

    float fThetaX = 2.5f;
    // Rotation X
    matRotX.m[0][0] = 1;
    matRotX.m[1][1] = cosf(fThetaX * 0.5f);
    matRotX.m[1][2] = sinf(fThetaX * 0.5f);
    matRotX.m[2][1] = -sinf(fThetaX * 0.5f);
    matRotX.m[2][2] = cosf(fThetaX * 0.5f);
    matRotX.m[3][3] = 1;

    // Store triagles for rastering later
    vector<triangle> vecTrianglesToRaster;


   /* matRotZ = IdentityMatrix();
    matRotX = IdentityMatrix();
    MatrixProj = IdentityMatrix();*/


    //OPTIONAL PAINTERS SORT
    std::sort(m.Tris.begin(), m.Tris.end(), [](const triangle& a, const triangle& b) {
        float zA = (a.p[0].z + a.p[1].z + a.p[2].z) / 3.0f;
        float zB = (b.p[0].z + b.p[1].z + b.p[2].z) / 3.0f;
       //OG winding order: return zA > zB;
        //flipped return zA < zB;
        return zA > zB;
    });

    for (auto tri : m.Tris)
    {
        triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

        // Rotate in Z-Axis
        MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
        MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
        MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

        // Rotate in X-Axis
        MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
        MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);



        // Offset into the screen
        float OffsetAmount = 2.0f;
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + OffsetAmount;
        triTranslated.p[1].z = triRotatedZX.p[1].z + OffsetAmount;
        triTranslated.p[2].z = triRotatedZX.p[2].z + OffsetAmount;

        //set mesh location -Experimental-BPA
        triTranslated.p[0].x = triTranslated.p[0].x + loc.x;
        triTranslated.p[0].y = triTranslated.p[0].y + loc.y;
        triTranslated.p[0].z = triTranslated.p[0].z + loc.z;

        triTranslated.p[1].x = triTranslated.p[1].x + loc.x;
        triTranslated.p[1].y = triTranslated.p[1].y + loc.y;
        triTranslated.p[1].z = triTranslated.p[1].z + loc.z;

        triTranslated.p[2].x = triTranslated.p[2].x + loc.x;
        triTranslated.p[2].y = triTranslated.p[2].y + loc.y;
        triTranslated.p[2].z = triTranslated.p[2].z + loc.z;


        // Project triangles from 3D --> 2D
        MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], MatrixProj);
        MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], MatrixProj);
        MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], MatrixProj);

        // Scale into view
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
        triProjected.p[0].x *= 0.5f * (float)SCREEN_X;
        triProjected.p[0].y *= 0.5f * (float)SCREEN_Y;
        triProjected.p[1].x *= 0.5f * (float)SCREEN_X;
        triProjected.p[1].y *= 0.5f * (float)SCREEN_Y;
        triProjected.p[2].x *= 0.5f * (float)SCREEN_X;
        triProjected.p[2].y *= 0.5f * (float)SCREEN_Y;

        // Store triangle for sorting
        vecTrianglesToRaster.push_back(triProjected);


       
    }

    // Sort triangles from back to front
    sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
    {
        float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
        float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
        return z1 > z2;
    });


    float depth = 0.0f;
    for (auto& triProjected : vecTrianglesToRaster)
    {
        // Distance shading
      //  float avgZ = (triTranslated.p[0].z + triTranslated.p[1].z + triTranslated.p[2].z) / 5.0f;
        float min = 1.0f;
        depth += 0.001f;
        // Clamp(1.0f - depth / min, 0.01f, 1.0f);
        depth = Clamp(depth, 0.01f, 1.5f);
        float brightness = depth;// ;



        RGB Red = { 255,0,0,255 };
        RGB Green = { 0,100,0,255 };
        RGB Blue = { 0,0,255,255 };
        RGB Dark = { 0,0,static_cast<int>(78 * brightness),255 };


        RGB MeshColor = {
            0,
            0,
            static_cast<int>(128 * brightness),
            255
        };



        /*
        * //YOU MIGHT BE ABLE TO CULL THE FACES DOING THIS IF YOU MAKE A CROSS AND DOT PRODUCT FUNCTION
        vec3d line1 = triTranslated.p[1] - triTranslated.p[0];
        vec3d line2 = triTranslated.p[2] - triTranslated.p[0];
        vec3d normal = CrossProduct(line1, line2);
        normal = Normalize(normal);

        vec3d cameraRay = triTranslated.p[0] - vec3d{ 0, 0, 0 }; // assuming camera at origin
        if (DotProduct(normal, cameraRay) < 0.0f) {
            // Triangle is visible—draw it
        }
        */


        //Rasterize Triangle
        Vertex p0 = { triProjected.p[0].x,triProjected.p[0].y,MeshColor };
        Vertex p1 = { triProjected.p[1].x,triProjected.p[1].y,MeshColor };
        Vertex p2 = { triProjected.p[2].x,triProjected.p[2].y,MeshColor };


        DrawTriangle(p0, p1, p2);
        int offset = 0;
        p0.x += offset;
        p0.y += offset;
        p1.x += offset;
        p1.y += offset;
        p2.x += offset;
        p2.y += offset;

        DrawLine(p0.x, p0.y, p1.x, p1.y, Dark);
        DrawLine(p1.x, p1.y, p2.x, p2.y, Dark);
        DrawLine(p2.x, p2.y, p0.x, p0.y, Dark);
    }
    

}
