#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chunk_str.h>
typedef unsigned char BYTE;
typedef struct
{
        BYTE    b;
        BYTE    g;
        BYTE    r;
} RGB_data;
void generate_island(ISLAND *island);
int bmp_generator(char *filename, int width, int height, unsigned char *data);
