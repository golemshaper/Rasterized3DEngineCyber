#pragma once
#include <cstdint>
#include <cstdio>
#include "DrawScratchSpace.h"

// --- BMP STRUCTS -------------------------------------------------------------

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType = 0x4D42; // 'BM'
    uint32_t bfSize = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;     // 14 + 40
};

struct DIBHeader {
    uint32_t biSize = 40;
    int32_t  biWidth = 0;
    int32_t  biHeight = 0;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24; // 24-bit RGB
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t  biXPelsPerMeter = 2835;
    int32_t  biYPelsPerMeter = 2835;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};
#pragma pack(pop)


// --- BMP WRITER --------------------------------------------------------------

inline bool WriteBMP(const char* filename, const RGB* pixels, int width, int height)
{
    if (!pixels || width <= 0 || height <= 0)
        return false;

    // BMP row padding: rows must be multiples of 4 bytes
    const int bytesPerPixel = 3; // B, G, R
    const int unpaddedRow = width * bytesPerPixel;
    const int padding = (4 - (unpaddedRow % 4)) % 4;
    const int rowSize = unpaddedRow + padding;
    const int dataSize = rowSize * height;

    BMPHeader bmp;
    DIBHeader dib;

    dib.biWidth = width;
    dib.biHeight = height; // bottom-up
    dib.biSizeImage = dataSize;

    bmp.bfSize = bmp.bfOffBits + dataSize;

    // Open file
    FILE* f = nullptr;
    if (fopen_s(&f, filename, "wb") != 0 || !f)
        return false;

    // Write headers
    fwrite(&bmp, sizeof(bmp), 1, f);
    fwrite(&dib, sizeof(dib), 1, f);

    // Temporary row buffer (tightly packed BGR)
    uint8_t* row = new uint8_t[rowSize];

    for (int y = height - 1; y >= 0; --y)
    {
        uint8_t* out = row;

        for (int x = 0; x < width; ++x)
        {
            const RGB& p = pixels[y * width + x];

            // Clamp to 0–255
            int R = p.r < 0 ? 0 : (p.r > 255 ? 255 : p.r);
            int G = p.g < 0 ? 0 : (p.g > 255 ? 255 : p.g);
            int B = p.b < 0 ? 0 : (p.b > 255 ? 255 : p.b);

            // BMP uses BGR order
            *out++ = (uint8_t)B;
            *out++ = (uint8_t)G;
            *out++ = (uint8_t)R;
        }

        // Padding bytes
        for (int i = 0; i < padding; i++)
            *out++ = 0;

        fwrite(row, rowSize, 1, f);
    }

    delete[] row;
    fclose(f);
    return true;
}