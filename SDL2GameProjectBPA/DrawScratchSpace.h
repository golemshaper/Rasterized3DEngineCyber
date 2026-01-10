#pragma once

#define SCREEN_X 240
#define SCREEN_Y 240//160

//#define SCREEN_X 135
//#define SCREEN_Y 240//160
#define TOTAL_PIXELS (SCREEN_X * SCREEN_Y)


#include <vector>
#include "InputWraper.h"
#include <cmath>
class TextSprites;

//STRUCTS
struct RGB {
    int r, g, b, a = 255;
    RGB operator+(const RGB& other) const {
        return {
            r + other.r,
            g + other.g,
            b + other.b,
            a + other.a
        };
    }
    RGB operator-(const RGB& other) const {
        return {
            r - other.r,
            g - other.g,
            b - other.b
        };
    }
    RGB operator*(const RGB& other) const {
        return {
            r * other.r,
            g * other.g,
            b * other.b,
            a* other.a
        };
    }

    RGB operator*(float other) const
    {
        auto clamp255 = [](float v) -> uint8_t {
            if (v < 0.0f) return 0;
            if (v > 255.0f) return 255;
            return static_cast<uint8_t>(v + 0.5f); // round instead of truncate
        };

        return {
            clamp255(r * other),
            clamp255(g * other),
            clamp255(b * other),
            clamp255(a * other)
        };
    }


    RGB operator/(const RGB& other) const {
        return {
            r / other.r,
            g / other.g,
            b / other.b,
            a / other.a
        };
    }
    RGB operator/(const int& other) const {
        return {
            r / other,
            g / other,
            b / other,
            a / other
        };
    }
    bool operator==(const RGB& other) const {
        return r == other.r && g == other.g && b == other.b && b == other.a;
    }
};

//2D
struct Point {
    int x, y;
};
struct Vertex {
    int x, y;
    RGB color;
    Vertex operator*(const int& other)const {
        return{
            x * other,
            y * other,
            color
        };
    }
    Vertex operator-(const int& other)const {
        return{
            x - other,
            y - other,
            color
        };
    }
};
struct Sprite {
    RGB* pixels;
    int width, height;
};
//3D
struct vec3d {
    float x, y, z;
    // Vector subtraction
    vec3d operator-(const vec3d& rhs) const
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }
    vec3d operator+(const vec3d& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }
    // Scalar multiplication
    vec3d operator*(float k) const
    {
        return { x * k, y * k, z * k };
    }

    vec3d operator*(vec3d k) const
    {
        return { x * k.x, y * k.y, z * k.z };
    }
    bool operator==(const vec3d& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    // Scalar division (optional but useful)
    vec3d operator/(float k) const
    {
        return { x / k, y / k, z / k };
    }
    float length_squared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(length_squared());
    }

};
struct triangle{
    vec3d p[3];
    float depth = 0.0f;
};

struct Mesh {
    std::vector<triangle> Tris;
};
struct mat4x4
{
    float m[4][4] = { 0 };
};

struct SortedMeshRecipe {
    //all of the data needed to render a differed model that is sorted by Z
    Mesh m;
    vec3d loc;
    vec3d rot; 
    vec3d scale;
    bool edge_light;
    RGB MeshColor;
};

//CLASS


class DrawScratchSpace {
public:
    RGB MainSpace[TOTAL_PIXELS]; //Screen X * Screen Y size
    RGB ExtraBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
    RGB ZBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
    void MoveMainspaceToExtraBuffer();
    void MultiplyBuffers();
    void AddBuffers();
    void AverageBuffers();
    void InvertBuffer();
    void ClearZBufffer();
    void DrawZBufffer();
    void ApplyMask();
    void BlendBuffers(float amount);
    void Initialize();
    void Initialize(RGB wipe);
    void Clear();
    void Clear(RGB wipe);
    void RandomScreenFill();
    void DrawRectangle(int x, int y, int width, int height, RGB color);
    void DrawRectangle(
        int x, int y,
        int width, int height,
        RGB topLeft,
        RGB topRight,
        RGB bottomLeft,
        RGB bottomRight);

    void DrawSquare(int x, int y, int size, RGB color);
    void DrawSquareMultiply(int x, int y, int size, RGB color);
    void DrawTriangle(Point p0, Point p1, Point p2, RGB color);
    void DrawTriangle(Vertex v0, Vertex v1, Vertex v2);
    void DrawTriangle(Vertex v0, Vertex v1, Vertex v2,int z);
    void DrawTriangleToZBuffer(Vertex v0, Vertex v1, Vertex v2,int z);
    void DrawCircle(int x, int y, int radius, RGB color);
    void DrawFilledCircle(int x, int y, int radius, RGB color);
    void DrawTriangleGlitchy(Vertex v0, Vertex v1, Vertex v2);
    RGB* ColorizeSpriteData(RGB* data, int spriteWidth, int spriteHeight, RGB color);
    void DrawSprite(int startX, int startY, Sprite sprite);
    void DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight);
    void DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight, bool wrap);
    void DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight,float angle);
    void DrawSpriteAdd(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight);
    void DrawText(int X, int Y, RGB color, const char* text, TextSprites* tSprites);
    void DrawText(int X, int Y, RGB color, const char* text, TextSprites* tSprites, float amount_revealed);



    void DrawLine(int x0, int y0, int x1, int y1, RGB color);
    int GetRandom(int a,int b);
    float Clamp(float value, float min, float max);
    RGB AlphaBlend(const RGB& dst, const RGB& src);
    RGB Lerp(RGB a, RGB b, float c);
    vec3d Lerp(vec3d a, vec3d b, float c);
    vec3d Arc(vec3d a, vec3d b, float h, float c);
    vec3d Arc(vec3d a, vec3d b, float h, float c,float direction);
    vec3d ArcHorizontal(vec3d a, vec3d b,float h, float c,float direction);
    Point RotatePoint(Point p, Point pivot, float angle);

//INPUT:
    InputWraper* Input = nullptr;
    vec3d GetMovementInput();
//HELPERS:
    const float PI = 3.141592653589793238462643383279502884F;
    int lastTextAmountRevealed = 0;
    std::vector<SortedMeshRecipe> recipes;
    unsigned int seed = 123456789;
    unsigned int GetNext();
    int GetNext(int min, int max);

//DRAW MODE:
    RGB MeshColor = { 255,255,255,255 };
    bool DrawVerticies = false;
    int EdgeBrightness = 255;
    bool DrawEdges = false;
    bool DrawHighlightEdgeOnly = false;
    bool ZWriteOn = false;
    int ZOffset = 0; //Used for illogical Drawing. Decals, Edge-lights and the like should use this
    float ZOffsetFloat = 0.0f; //Used for illogical Drawing. Decals, Edge-lights and the like should use this
//3D Primitives:
    Mesh MeshCube;
    mat4x4 MatrixProj;
    mat4x4 StoredCameraMatView;
    float fTheta;
    vec3d LastLocation;
    vec3d CameraLoc;
    vec3d CameraTargetLoc;
//3D Functions:
    void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);
    mat4x4 IdentityMatrix();
    mat4x4 Matrix_PointAt(vec3d pos, vec3d target, vec3d up);
    mat4x4 Matrix_QuickInverse(mat4x4& m);
    mat4x4 CalculateViewMatrix();
    void SetCamera(vec3d loc, vec3d target);
    void SetCameraFOV(float nFov);
    void DrawMesh(Mesh m);
    void DrawMesh(Mesh m,vec3d loc);
    void DrawMesh(Mesh m, vec3d loc, vec3d rot);
    void DrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale);
    void DrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale, bool edge_light);
    void DifferDrawMesh(Mesh m);
    void DifferDrawMesh(Mesh m, vec3d loc);
    void DifferDrawMesh(Mesh m, vec3d loc, vec3d rot);
    void DifferDrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale);
    void DifferDrawMesh(Mesh m, vec3d loc, vec3d rot, vec3d scale, bool edge_light);
    void DrawSortedDifferedMeshes();

    void DrawSprite3D(Sprite s, vec3d loc, vec3d rot, vec3d scale);

    vec3d Get2DPointInFromSpace(vec3d loc);
    vec3d Get2DPointFromLastLocation();
    vec3d Normalize(vec3d input);
    vec3d CrossProduct(const vec3d& a, const vec3d& b);
    vec3d LookAtRotation(const vec3d& from, const vec3d& to);
    vec3d LookAtRotation2D(const vec3d& from, const vec3d& to);
    float Distance(const vec3d& a, const vec3d& b);
    float Distance2D(const vec3d& a, const vec3d& b);
    float SquaredDistance(const vec3d& a, const vec3d& b);
    float SquaredDistance2D(const vec3d& a, const vec3d& b);
 //FX
    void SetFade(RGB color, float a);
    void SetFade(
        RGB topLeft,
        RGB topRight,
        RGB bottomLeft,
        RGB bottomRight, 
        float a
    );


};