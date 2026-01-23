#pragma once
#include <cstdint>
#include <cstdio>
#include "DrawScratchSpace.h"
#pragma pack(push, 1)
struct BMPHeader2 {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct DIBHeader2 {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)


inline RGB* ReadBMP(const char* filename, int& outW, int& outH)
{
    FILE* f = nullptr;
    if (fopen_s(&f, filename, "rb") != 0 || !f)
        return nullptr;

    BMPHeader2 bmp;
    DIBHeader2 dib;

    fread(&bmp, sizeof(bmp), 1, f);
    fread(&dib, sizeof(dib), 1, f);

    // Validate minimal BMP
    if (bmp.bfType != 0x4D42 || dib.biBitCount != 24 || dib.biCompression != 0) {
        fclose(f);
        return nullptr;
    }

    const int width = dib.biWidth;
    const int height = dib.biHeight;
    outW = width;
    outH = height;

    const int bytesPerPixel = 3;
    const int unpaddedRow = width * bytesPerPixel;
    const int padding = (4 - (unpaddedRow % 4)) % 4;
    const int rowSize = unpaddedRow + padding;

    // Allocate pixels
    RGB* pixels = new RGB[width * height];

    // Seek to pixel array
    fseek(f, bmp.bfOffBits, SEEK_SET);

    uint8_t* row = new uint8_t[rowSize];

    // BMP is bottom-up
    for (int y = height - 1; y >= 0; --y)
    {
        fread(row, rowSize, 1, f);
        const uint8_t* in = row;

        for (int x = 0; x < width; ++x)
        {
            uint8_t B = *in++;
            uint8_t G = *in++;
            uint8_t R = *in++;

            RGB& p = pixels[y * width + x];
            p.r = R;
            p.g = G;
            p.b = B;
            p.a = 255;
        }
    }

    delete[] row;
    fclose(f);
    return pixels;
}