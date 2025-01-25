#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct bmp {
    char extraMasks[16];
} bmp;

typedef enum {
    TYPE_BITMAPCOREHEADER=12,
    TYPE_OS22XBITMAPHEADER=64,
    TYPE_BITMAPINFOHEADER=40,
    TYPE_BITMAPV2INFOHEADER=52,
    TYPE_BITMAPV3INFOHEADER=56,
    TYPE_BITMAPV4HEADER=108,
    TYPE_BITMAPV5HEADER=124
} DIBHeaders;

typedef enum {
    TYPE_BI_RGB,
    TYPE_BI_RLE8,
    TYPE_BI_RLE4,
    TYPE_BI_BITFIELDS,
    TYPE_BI_JPEG,
    TYPE_BI_PNG,
    TYPE_BI_ALPHABITFIELDS,
    TYPE_BI_CMYK = 11,
    TYPE_BI_CMYKRLE8,
    TYPE_BI_CMYKRLE4
} CompressionIdentifier;

bmp* readBMP(char name[]);

int main(void) {
    bmp* buffer = readBMP("img.bmp");

    free(buffer);
    return 0;
}


bmp* readBMP(char name[]) {
    FILE* file = fopen(name, "rb");
    //fseek(file, 0, SEEK_END);
    //const unsigned long fileLen = ftell(file);
    //rewind(file);
    //return malloc(fileLen*sizeof(char));

    //BITMAP HEADER

    //First 2 bytes
    //offset 0
    char title[2];
    fread(title, sizeof(char), 2, file);
    if (!strcmp(title, "BM")) {
        // Windows 3.1x, 95, NT, ... etc.
    } else if (!strcmp(title, "BA")) {
        // OS/2 struct bitmap array
    } else if (!strcmp(title, "CI")) {
        // OS/2 struct color icon
    } else if (!strcmp(title, "CP")) {
        // OS/2 const color pointer
    } else if (!strcmp(title, "IC")) {
        // OS/2 struct icon
    } else if (!strcmp(title, "PT")) {
        // OS/2 pointer
    } else {
        //Unrecognized file header
    }

    //File size
    //offset 2
    unsigned int size;
    fread(&size, 4, 1, file);

    //Skipping reserved bytes
    //offset 6
    fseek(file, 4, SEEK_CUR);

    //Offset
    //offset 10
    unsigned int offset;
    fread(&offset, 4, 1, file);

    //DIB HEADER


    //DIB Header size
    //All headers have a size=dibSize except for when dibSize=16
    //Setting types
    //offset 14
    DWORD dibSize;
    fread(&dibSize, sizeof(DWORD), 1, file);

    //FROM THIS POINT ON, ONLY BITMAPINFOHEADER AND SUCCESSIVE VERSIONS ARE IMPLEMENTED

    //Width in pixels
    //offset 18
    DWORD width;
    fread(&width, sizeof(DWORD), 1, file);

    //Height in pixels
    //offset 22
    DWORD height;
    fread(&height, sizeof(DWORD), 1, file);

    //Number of color planes
    //Must be 1 if header specifically is BITMAPINFOHEADER
    //offset 26
    WORD colorPlanes;
    fread(&colorPlanes, sizeof(WORD), 1, file);

    //Color depth
    //offset 28
    WORD colorDepth;
    fread(&colorDepth, sizeof(WORD), 1, file);

    //Compression method being used
    //offset 30
    DWORD compression;
    fread(&compression, sizeof(DWORD), 1, file);

    //Size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
    //offset 34
    DWORD dataSize;
    fread(&dataSize, sizeof(DWORD), 1, file);

    //Horizontal resolution of the image. (pixel per metre, signed integer)
    //offset 38
    DWORD horizontalRes;
    fread(&horizontalRes, sizeof(DWORD), 1, file);

    //Vertical resolution of the image. (pixel per metre, signed integer)
    //offset 42
    DWORD verticalRes;
    fread(&verticalRes, sizeof(DWORD), 1, file);

    //The number of colors in the color palette, or 0 to default to 2n
    //offset 46
    DWORD colorNum;
    fread(&colorNum, sizeof(DWORD), 1, file);

    //The number of important colors used, or 0 when every color is important; generally ignored
    //offset 50
    DWORD importantColors;
    fread(&importantColors, sizeof(DWORD), 1, file);
}