#include <cglm/ivec2.h>
#include <merchant_str.h>
#include <enemy_str.h>
/*
                                CHUNK_STR.h
Describes the struct representing an exploration mode chunk, and the struct
representing an exploration mode island. Can be included in any file which
reads/manipulates chunks and/or islands.
*/

#define C_WIDTH (512) // Width of a chunk in tiles
#define I_WIDTH (128) // Width of an island in tiles
#define MAX_ISLANDS (3) // Max islands per chunk

typedef enum tile {
  OCEAN = 0,
  SHORE = 1,
  SAND = 2,
  GRASS = 3,
  ROCK = 4
} TILE;

typedef struct island {
  ivec2 chunk;
  ivec2 coords;
  TILE tiles[I_WIDTH * I_WIDTH];
  MERCHANT merchant;
  int has_merchant;
} ISLAND;

typedef struct chunk {
  ivec2 coords;
  ISLAND islands[MAX_ISLANDS];
  E_ENEMY *enemies;
  unsigned int num_islands;
  unsigned int num_enemies;
} CHUNK;
