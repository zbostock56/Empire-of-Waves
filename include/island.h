/*
                                ISLAND.h
In tandem with chunk_str.h, this header defines functions whose role is to
procedurally generate islands and populate structs defined in other headers
so that the graphics side can render the islands in the viewport.
*/

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <math.h>
#include <glad/glad.h>
#include <chunk_str.h>
#include <items.h>

/* DEFINES */
#define FREQ          (0.05)
#define DEPTH         (3)
#define X             (0)
#define Y             (1)
#define MAX_INT       (2147483647)
#define MAX_INT_FLOAT (2147483647.0)
#define TRUE          (1)
#define FALSE         (0)

/* PROTOTYPES */
/* perlin.h */
float perlin(float, float, float, int, int);

/* Native functions */
int generate_island(ISLAND *);
int generate_rand();
double nano_time();
void generate_mask(float (*mask)[I_WIDTH]);
void populate_tiles(ISLAND *, float (*)[I_WIDTH]);
void merchant_generate(MERCHANT *, ISLAND *);
unsigned int texture_from_buffer(unsigned char *, int, int, int);

/* MACROS */
/* Safe-guard for the chances if tile_location is */
/* set to less than zero */
#define get_tile_location(tile, island_width) {                 \
  tile = generate_rand() % (island_width * island_width) - 1;   \
  while (tile < 0) {                                            \
    tile = generate_rand() % (island_width * island_width) - 1; \
  }                                                             \
}
