#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct bmp {
    char extraMasks[16];
} bmp;

typedef union {
    BITMAPCOREHEADER bitmapcoreheader;
    BITMAPV5HEADER bitmapv5_header;
} headerTypes;

typedef struct {
    headerTypes value;
    int type;
} header;

enum {
    TYPE_BITMAPCOREHEADER,
    TYPE_OS22XBITMAPHEADER,
    TYPE_BITMAPINFOHEADER,
    TYPE_BITMAPV2INFOHEADER,
    TYPE_BITMAPV3INFOHEADER,
    TYPE_BITMAPV4HEADER,
    TYPE_BITMAPV5HEADER
};

bmp* readBMP(char name[]);

int main(void) {
    char* buffer = readBMP("img.bmp");

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
    unsigned int size;
    fread(&size, 4, 1, file);

    //Skipping reserved bytes
    fseek(file, 4, SEEK_CUR);

    //Offset
    unsigned int offset;
    fread(&offset, 4, 1, file);

    //DIB Header
    header dibHeader;

    //DIB Header size
    unsigned int dibSize;
    fread(&dibSize, sizeof(DWORD), 1, file);

    switch (dibSize) {
        case 12:
            //BITMAPCOREHEADER
            //OS21XBITMAPHEADER
            dibHeader.type = TYPE_BITMAPCOREHEADER;
            dibHeader.value
            break;
        case 64:
            //OS22XBITMAPHEADER
            break;
        case 16:
            //OS22XBITMAPHEADER
            //Same as last one but the rest of the values are assumed to be 0
            break;
        case 40:
            //BITMAPINFOHEADER
            break;
        case 52:
            //BITMAPV2INFOHEADER
            break;
        case 56:
            //BITMAPV3INFOHEADER
            break;
        case 108:
            //BITMAPV4HEADER
                break;
        case 124:
            //BITMAPV5HEADER
            break;
        default:
            //Unrecognized DIB header
    }
}