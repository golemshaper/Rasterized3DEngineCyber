#pragma once

#define SCREEN_X 240
#define SCREEN_Y 160
#define TOTAL_PIXELS (SCREEN_X * SCREEN_Y)


using namespace std; 
#include <vector>


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
    RGB operator/(const RGB& other) const {
        return {
            r / other.r,
            g / other.g,
            b / other.b,
            a / other.a
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
};

//3D
struct vec3d {
    float x, y, z;
};
struct triangle{
    vec3d p[3];
};

struct Mesh {
    vector<triangle> Tris;
};
struct mat4x4
{
    float m[4][4] = { 0 };
};


//CLASS

class DrawScratchSpace {
public:
    RGB MainSpace[TOTAL_PIXELS]; //Screen X * Screen Y size
    RGB ExtraBuffer[TOTAL_PIXELS]; //Screen X * Screen Y size
    void MoveMainspaceToExtraBuffer();
    void MultiplyBuffers();
    void AddBuffers();
    void Initialize();
    void Initialize(RGB wipe);
    void Clear();
    void Clear(RGB wipe);
    void RandomScreenFill();
    void DrawSquare(int x, int y, int size, RGB color);
    void DrawSquareMultiply(int x, int y, int size, RGB color);
    void DrawTriangle(Point p0, Point p1, Point p2, RGB color);
    void DrawTriangle(Vertex v0, Vertex v1, Vertex v2);
    void DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight);
    void DrawSprite(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight,float angle);
    void DrawSpriteAdd(int startX, int startY, RGB* SpriteData, int spriteWidth, int spriteHeight);
    void DrawLine(int x0, int y0, int x1, int y1, RGB color);
    int GetRandom(int a,int b);
    Point RotatePoint(Point p, Point pivot, float angle);


//3D Primitives:
    Mesh MeshCube;
    mat4x4 MatrixProj;
    float fTheta;
//3D Functions:
    void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);
    mat4x4 IdentityMatrix();
    void DrawMesh(Mesh m,float fElapsedTime);

};