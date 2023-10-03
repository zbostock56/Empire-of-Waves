#include <player_str.h>
#include <chunk_str.h>

/* GLOBALS  */
extern CHUNK player_chunks[CHUNKS_SIMULATED];
extern E_PLAYER e_player;

/* DEFINES */
#define CURRENT_CHUNK (4)

/* MACROS */
#define convert_chunk_coords(pos, chunk, player_chunk) {\
  if (pos == 0) {                                       \
    chunk[0] = player_chunk[0] - 1;                     \
    chunk[1] = player_chunk[1] + 1;                     \
  } else if (pos == 1) {                                \
    chunk[0] = player_chunk[0];                         \
    chunk[1] = player_chunk[1] + 1;                     \
  } else if (pos == 2) {                                \
    chunk[0] = player_chunk[0] + 1;                     \
    chunk[1] = player_chunk[1] + 1;                     \
  } else if (pos == 3) {                                \
    chunk[0] = player_chunk[0] - 1;                     \
    chunk[1] = player_chunk[1];                         \
  } else if (pos == 4) {                                \
    chunk[0] = player_chunk[0];                         \
    chunk[1] = player_chunk[1];                         \
  } else if (pos == 5) {                                \
    chunk[0] = player_chunk[0] + 1;                     \
    chunk[1] = player_chunk[1];                         \
  } else if (pos == 6) {                                \
    chunk[0] = player_chunk[0] - 1;                     \
    chunk[1] = player_chunk[1] - 1;                     \
  } else if (pos == 7) {                                \
    chunk[0] = player_chunk[0];                         \
    chunk[1] = player_chunk[1] - 1;                     \
  } else if (pos == 8) {                                \
    chunk[0] = player_chunk[0] + 1;                     \
    chunk[1] = player_chunk[1] - 1;                     \
  }                                                     \
}

/* =================== INTERNALLY DEFINED FUNCTIONS =================== */
void spawn_enemy(ivec2 chunk, ivec2 coords);
int find_avail_chunks();
void generate_chunk_tiles(int chunk_tiles[C_WIDTH][C_WIDTH], CHUNK chunk);

/* =================== EXTERNALLY DEFINED FUNCTIONS =================== */
