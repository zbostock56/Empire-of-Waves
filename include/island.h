/*
                                ISLAND.h
In tandem with chunk_str.h, this header defines functions whose role is to
procedurally generate islands and populate structs defined in other headers
so that the graphics side can render the islands in the viewport.
*/

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <chunk_str.h>
#include <time.h>
#include <error.h>
#include <math.h>
#include <string.h>

/* DEFINES */
#define FREQ (0.1)
#define DEPTH (4)

/* PROTOTYPES */
/* perlin.h */
float perlin(float x, float y, float freq, int depth, int SEED);

/* Native functions */
void generate_island(ISLAND *island);
int generate_rand();
double nano_time();
void generate_mask(float (*mask)[I_WIDTH]);
void populate_tiles(ISLAND *island, float (*pnoise)[I_WIDTH]);
