#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>

#define MAX_SAVE_NAME_LEN (32)

// ========================= GLOBAL GAME STATE ===============================
typedef enum game_mode {
  EXPLORATION,
  COMBAT
} GAME_MODE;
extern GAME_MODE mode;
extern float delta_time;
extern float last_frame;
extern char save_name[MAX_SAVE_NAME_LEN];

// ======================== EXPLORATION MODE STATE ===========================
extern CHUNK *chunk_buffer;
extern unsigned int chunk_buff_size;
extern unsigned int chunk_buff_len;

extern E_PLAYER e_player;
// Indexs to chunks in chunk_buffer which represent the chunks loaded by the
// player
extern unsigned int player_chunks[9];
// Utility buffer for tracking updated indices of chunks in chunk_buffer, which
// represent the loaded player chunks
extern unsigned int updated_chunks[9];
extern vec2 home_island_coords;
extern vec2 house_tile;
extern TRADE_SHIP *trade_ships;
extern unsigned int num_trade_ships;
extern unsigned int trade_ship_buf_size;
extern float next_event;
extern MERCHANT *cur_merchant;

// ========================= COMBAT MODE STATE ===============================
extern C_PLAYER c_player;
extern C_UNIT *npc_units;
// Index of enemy ship in chunk buffer which is currently engaged in combat
// with player
extern unsigned int e_enemy_index;
extern unsigned int num_npc_units;
// Specifies width and height of game arena in tiles
extern ivec2 arena_dimensions;
