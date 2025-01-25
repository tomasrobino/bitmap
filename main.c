#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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

void readBMP(char name[]);

int main(void) {
    readBMP("img.bmp");

    return 0;
}


void readBMP(char name[]) {
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
    unsigned int dibSize;
    fread(&dibSize, sizeof(unsigned int), 1, file);

    //FROM THIS POINT ON, ONLY BITMAPINFOHEADER AND SUCCESSIVE VERSIONS ARE IMPLEMENTED

    //Width in pixels
    //offset 18
    int width;
    fread(&width, sizeof(int), 1, file);

    //Height in pixels
    //offset 22
    int height;
    fread(&height, sizeof(int), 1, file);

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
    unsigned int compression;
    fread(&compression, sizeof(unsigned int), 1, file);

    //Size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
    //offset 34
    unsigned int dataSize;
    fread(&dataSize, sizeof(unsigned int), 1, file);

    //Horizontal resolution of the image. (pixel per metre, signed integer)
    //offset 38
    int horizontalRes;
    fread(&horizontalRes, sizeof(int), 1, file);

    //Vertical resolution of the image. (pixel per metre, signed integer)
    //offset 42
    int verticalRes;
    fread(&verticalRes, sizeof(int), 1, file);

    //The number of colors in the color palette, or 0 to default to 2^n
    //offset 46
    unsigned int colorNum;
    fread(&colorNum, sizeof(unsigned int), 1, file);

    //The number of important colors used, or 0 when every color is important; generally ignored
    //offset 50
    unsigned int importantColors;
    fread(&importantColors, sizeof(unsigned int), 1, file);

    //ONLY FOR BITMAPV4HEADER
    if (size==108 || size==124) {
        //red, green and blue Masks, specifies the color component of each pixel,
        //valid only if compression is set to BI_BITFIELDS.
        //offset 54
        unsigned int redMask;
        fread(&redMask, sizeof(unsigned int), 1, file);
        //offset 58
        unsigned int greenMask;
        fread(&greenMask, sizeof(unsigned int), 1, file);
        //offset 62
        unsigned int blueMask;
        fread(&blueMask, sizeof(unsigned int), 1, file);
        //Color mask that specifies the alpha component of each pixel
        //offset 66
        unsigned int alphaMask;
        fread(&alphaMask, sizeof(unsigned int), 1, file);
        //The color space of the DIB. Only possible value is LCS_CALIBRATED_RGB
        //offset 70
        unsigned int colorSpace;
        fread(&colorSpace, sizeof(unsigned int), 1, file);
        //A CIEXYZTRIPLE structure that specifies the x, y, and z coordinates
        //of the three colors that correspond to the red, green, and blue endpoints for the logical
        //color space associated with the bitmap.
        //This member is ignored unless the colorSpace member specifies LCS_CALIBRATED_RGB.
        //offset 74
        CIEXYZTRIPLE endpoints;
        fread(&endpoints, sizeof(CIEXYZTRIPLE), 1, file);
        //Tone response curves for red, green and blue.
        //These members are ignored unless color values are calibrated RGB values
        //and colorSpace is set to LCS_CALIBRATED_RGB.
        //offset 110
        unsigned int redGamma;
        fread(&redGamma, sizeof(unsigned int), 1, file);
        //offset 114
        unsigned int greenGamma;
        fread(&greenGamma, sizeof(unsigned int), 1, file);
        //offset 118
        unsigned int blueGamma;
        fread(&blueGamma, sizeof(unsigned int), 1, file);

    }

    if (size==124) {
        //Rendering intent for bitmap
        //offset 122
        unsigned int intent;
        fread(&intent, sizeof(unsigned int), 1, file);
        //The offset, in bytes, from the beginning of the BITMAPV5HEADER structure to the start of the profile data
        //offset 126
        unsigned int profileData;
        fread(&profileData, sizeof(unsigned int), 1, file);
        //Size, in bytes, of embedded profile data
        //offset 130
        unsigned int profileSize;
        fread(&profileSize, sizeof(unsigned int), 1, file);
        //Skipping reserved bytes
        //offset 134
        fseek(file, 4, SEEK_CUR);
    }
}