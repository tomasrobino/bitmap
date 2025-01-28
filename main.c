#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

typedef struct {
    int ciexyzX : 32;
    int ciexyzY : 32;
    int ciexyzZ : 32;
} CIEXYZ;

typedef struct {
    CIEXYZ ciexyzRed;
    CIEXYZ ciexyzGreen;
    CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
    DIBHeaders dibSize : 32;
    int width : 32;
    int height: 32;
    unsigned short colorPlanes : 16;
    unsigned short bitCount : 16;
    CompressionIdentifier compression : 32;
    unsigned int sizeImage : 32;
    int horizontalRes : 32;
    int verticalRes :32;
    unsigned int colorNum : 32;
    unsigned int importantColors : 32;
} headerV1;

typedef struct {
    DIBHeaders dibSize : 32;
    int width : 32;
    int height: 32;
    unsigned short colorPlanes : 16;
    unsigned short bitCount : 16;
    CompressionIdentifier compression : 32;
    unsigned int sizeImage : 32;
    int horizontalRes : 32;
    int verticalRes :32;
    unsigned int colorNum : 32;
    unsigned int importantColors : 32;
    unsigned int redMask : 32;
    unsigned int greenMask : 32;
    unsigned int blueMask : 32;
    unsigned int alphaMask : 32;
    unsigned int colorSpace : 32;
    CIEXYZTRIPLE endpoints;
    unsigned int redGamma : 32;
    unsigned int greenGamma : 32;
    unsigned int blueGamma : 32;
} headerV4;

typedef struct {
    DIBHeaders dibSize : 32;
    int width : 32;
    int height: 32;
    unsigned short colorPlanes : 16;
    unsigned short bitCount : 16;
    CompressionIdentifier compression : 32;
    unsigned int sizeImage : 32;
    int horizontalRes : 32;
    int verticalRes :32;
    unsigned int colorNum : 32;
    unsigned int importantColors : 32;
    unsigned int redMask : 32;
    unsigned int greenMask : 32;
    unsigned int blueMask : 32;
    unsigned int alphaMask : 32;
    unsigned int colorSpace : 32;
    CIEXYZTRIPLE endpoints;
    unsigned int redGamma : 32;
    unsigned int greenGamma : 32;
    unsigned int blueGamma : 32;
    unsigned int intent : 32;
    unsigned int profileData : 32;
    unsigned int profileSize : 32;
    unsigned int reserved : 32;
} headerV5;

void readBMP(char name[]);
headerV1* buildHeader(FILE* file);
void make_gap(FILE* file);
char* readUncompressed(int width, int bitCount, FILE* file);
char* readRLE8(int height, int width, FILE* file);
void print_header_v1(const headerV1* header);
void print_header_v4(headerV4* header);
void print_header_v5(headerV5* header);

int main(void) {
    readBMP("example.bmp");

    return 0;
}


headerV1* buildHeader(FILE* file) {
    //BITMAP HEADER

    //First 2 bytes
    //offset 0
    char title[2];
    fread(title, sizeof(char), 2, file);
    if (strcmp(title, "BM") != 0) {
        //Unrecognized file header
        return NULL;
    }

    //File size
    //offset 2
    unsigned int fileSize;
    fread(&fileSize, 4, 1, file);

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
    unsigned short colorPlanes;
    fread(&colorPlanes, sizeof(short), 1, file);
    if (colorPlanes != 1) {
        //Invalid value
        //Moving pointer to end of header
        fseek(file, (long)dibSize-24, SEEK_CUR);
        return NULL;
    }

    //Color depth
    //offset 28
    unsigned short bitCount;
    fread(&bitCount, sizeof(short), 1, file);

    //Compression method being used
    //offset 30
    CompressionIdentifier compression;
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

    headerV1* header = malloc(sizeof(headerV1));
    header->dibSize=dibSize;
    header->width=width;
    header->height=height;
    header->colorPlanes=colorPlanes;
    header->bitCount=bitCount;
    header->compression=compression;
    header->sizeImage=dataSize;
    header->horizontalRes=horizontalRes;
    header->verticalRes=verticalRes;
    header->colorNum=colorNum;
    header->importantColors=importantColors;
    return header;
}

void readBMP(char name[]) {
    FILE* file = fopen(name, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    //fseek(file, 0, SEEK_END);
    //const unsigned long fileLen = ftell(file);
    //rewind(file);
    //return malloc(fileLen*sizeof(char));

    //Building DIB header structure
    headerV1* header = buildHeader(file);
    if (header == NULL) {
        fclose(file);
        free(header);
        return;
    }
    print_header_v1(header);

    //COLOR MASKS
    unsigned int colorMasks[3];
    if (header->compression == TYPE_BI_BITFIELDS) {
        fread(&colorMasks, sizeof(unsigned int), 3, file);
    }

    //COLOR TABLE
    unsigned long colorTableEntries = header->colorNum;
    if (header->bitCount <= 8 || header->colorNum>0) {
        //Color table immediately following DIB header
        //Array of RGBQUAD. Size is given by colorNum.
        //If colorNum is 0, the array contains the maximum number of colors
        //for the given bitdepth; that is, 2^bitCount colors
        if (colorTableEntries==0) {
            colorTableEntries = pow(2, header->bitCount);
        }
    }
    //colorTableEntries should be zero if there's no color table (or if it has maximum colors)
    unsigned int colorTable[colorTableEntries][4];
    for (int i = 0; i < colorTableEntries; i++) {
        fread(&colorTable[i], sizeof(unsigned int), 4, file);
    }
    
    //GAP 1
    make_gap(file);

    //PIXEL TABLE
    //If header->height > 0, then rows are bottom up
    if (header->height > 0) {
        for (int i = header->height; i > 0; i--) {
            
        }
    } else {
        for (int i = 0; i < header->height; i++) {
            
        }
    }
    

    free(header);
    fclose(file);
}

void make_gap(FILE* file) {
    if (ftell(file)%4 != 0) {
        fseek(file, 4-(ftell(file)%4), SEEK_CUR);
    }
}

char* readUncompressed(int width, int bitCount, FILE* file) {
    //stride = ((((biWidth * biBitCount) + 31) & ~31) >> 3);
    //biSizeImage = abs(biHeight) * stride;
    //int stride = floor((width * bitCount + 31) / 32) * 4;
    if (bitCount == 1) {
        //1 bit per pixel (bpp)
        char* row = malloc(ceil(width/8));
        fread(row, 1, width/8 +1, file);
        make_gap(file);
        return row;
    } else if (bitCount == 4) {
        //4 bits per pixel
        char* row = malloc(ceil(width/2));
        fread(row, 1, width/2 +1, file);
        make_gap(file);
        return row;
    } else if (bitCount == 8) {
        //1 byte per pixel
        char* row = malloc(width);
        fread(row, 1, width, file);
        make_gap(file);
        return row;
    } else if (bitCount == 16) {
        //2 bytes per pixel
        unsigned short row = malloc(width*2);
        fread(row, 2, width, file);
        make_gap(file);
        return row;
    } else if (bitCount == 32) {
        //4 bytes per pixel
        unsigned int row = malloc(width*4);
        fread(row, 4, width, file);
        return row;
    }
}

char* readRLE8(int height, int width, FILE* file) {
    //Only works for 8bpp
    char rep = fgetc(file);
    if ( rep == 0) {
        //Absolute mode
    } else {
        //Encoded mode
        char* pixelArray = malloc(height*width);
        /*
        int pixelCount = 0;
        int pixelAux = rep;
        int end = 0;
        while(!end) {
            while(pixelAux != 0) {
                char color = fgetc(file);
                for (int i = 0; i < pixelAux; i++) {
                    pixelArray[i+pixelCount] = color;
                }
                pixelCount += pixelAux;
                pixelAux = fgetc(file);
                if (pixelAux == 0) {
                    char sec = fgetc(file);
                    if (sec == 0) {
                        //End of row
                        for (int j = 0; j < width-pixelCount; j++) {
                            pixelArray[j+pixelCount] = 0;
                        }
                        
                    }
                }
            }
        }
        */
        int end = 0;
        int pixelTotal = 0;
        int firstByte = rep;
        while(end != 1) {
            char secondByte = fgetc(file);
            if (firstByte == 0 && secondByte == 0) {
                //End of current row
                pixelArray[pixelTotal] = '\n';
                pixelTotal++;
            } else if (firstByte == 0 && secondByte == 1) {
                //End of bitmap
                end = 1;
            } else if (firstByte == 0 && secondByte == 2) {
                //Delta offset
                char thirdByte = fgetc(file);
                char fourthByte = fgetc(file);
                int delta = fourthByte*width + thirdByte;
                pixelTotal+=delta;
                for (int i = 0; i < delta; i++) {
                    pixelArray[i+pixelTotal] = secondByte;
                }
                pixelTotal+=firstByte;
            } else {
                for (int i = 0; i < firstByte; i++) {
                    pixelArray[i+pixelTotal] = secondByte;
                }
                pixelTotal+=firstByte;
            }
        }
        
    }
}

void print_header_v1(const headerV1* header) {
    printf("\n");
    printf("dibSize %d\n", header->dibSize);
    printf("width %d\n", header->width);
    printf("height %d\n", header->height);
    printf("colorPlanes %d\n", header->colorPlanes);
    printf("bitCount %d\n", header->bitCount);
    printf("compression %d\n", header->compression);
    printf("sizeImage %d\n", header->sizeImage);
    printf("horizontalRes %d\n", header->horizontalRes);
    printf("verticalRes %d\n", header->verticalRes);
    printf("colorNum %d\n", header->colorNum);
    printf("importantColors %d\n", header->importantColors);
}
void print_header_v4(headerV4* header) {
    print_header_v1((headerV1*)header);
    printf("redMask %d\n", header->redMask);
    printf("greenMask %d\n", header->greenMask);
    printf("blueMask %d\n", header->blueMask);
    printf("alphaMask %d\n", header->alphaMask);
    printf("colorSpace %d\n", header->colorSpace);
    printf("endpoints %d\n", header->endpoints.ciexyzRed.ciexyzX);
    printf("endpoints %d\n", header->endpoints.ciexyzRed.ciexyzY);
    printf("endpoints %d\n", header->endpoints.ciexyzRed.ciexyzZ);
    printf("endpoints %d\n", header->endpoints.ciexyzGreen.ciexyzX);
    printf("endpoints %d\n", header->endpoints.ciexyzGreen.ciexyzY);
    printf("endpoints %d\n", header->endpoints.ciexyzGreen.ciexyzZ);
    printf("endpoints %d\n", header->endpoints.ciexyzBlue.ciexyzX);
    printf("endpoints %d\n", header->endpoints.ciexyzBlue.ciexyzY);
    printf("endpoints %d\n", header->endpoints.ciexyzBlue.ciexyzZ);
    printf("redGamma %d\n", header->redGamma);
    printf("greenGamma %d\n", header->greenGamma);
    printf("blueGamma %d\n", header->blueGamma);
}
void print_header_v5(headerV5* header) {
    print_header_v4((headerV4*)header);
    printf("intent %d\n", header->intent);
    printf("profileData %d\n", header->profileData);
    printf("profileSize %d\n", header->profileSize);
    printf("reserved %d\n", header->reserved);
}