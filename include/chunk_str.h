#include <cglm/ivec2.h>
#include <merchant_str.h>
#include <enemy_str.h>
/*
                                CHUNK_STR.h
Describes the struct representing an exploration mode chunk, and the struct
representing an exploration mode island. Can be included in any file which
reads/manipulates chunks and/or islands.
*/

#define C_WIDTH (128) // Width of a chunk in tiles
#define I_WIDTH (96) // Width of an island in tiles
#define MAX_ISLANDS (2) // Max islands per chunk
#define CHUNKS_SIMULATED (9)

// Macros specifying index names for player_chunks
#define CHUNK_UPPER_LEFT (0)
#define CHUNK_UP (1)
#define CHUNK_UPPER_RIGHT (2)
#define CHUNK_LEFT (3)
#define PLAYER_CHUNK (4)
#define CHUNK_RIGHT (5)
#define CHUNK_LOWER_LEFT (6)
#define CHUNK_DOWN (7)
#define CHUNK_LOWER_RIGHT (8)

// Offsets of each chunk in player_chunks from player_chunks[4]
// Each index correponds to the same chunk pointer in player_chunks

extern float T_WIDTH;
extern ivec2 CHUNK_OFFSETS[CHUNKS_SIMULATED];

typedef enum tile {
  OCEAN = 0,
  SHORE = 1,
  SAND = 2,
  GRASS = 3,
  ROCK = 4,
  MERCH = 5
} TILE;

typedef struct island {
  ivec2 chunk;
  ivec2 coords;
  TILE tiles[I_WIDTH * I_WIDTH];
  MERCHANT merchant;
  int has_merchant;
  unsigned int texture;
} ISLAND;

typedef struct chunk {
  ivec2 coords;
  ISLAND islands[MAX_ISLANDS];
  E_ENEMY *enemies;
  unsigned int num_islands;
  unsigned int num_enemies;
} CHUNK;
