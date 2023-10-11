// GLOBAL GAME STATE
typedef enum game_mode {
  EXPLORATION,
  COMBAT
} GAME_MODE;
extern GAME_MODE mode;
extern float delta_time;
extern float last_frame;

// EXPLORATION MODE STATE
extern E_PLAYER e_player;
extern CHUNK player_chunks[9];
extern vec2 home_island_coords;
extern TRADE_SHIP *trade_ships;
extern unsigned int num_trade_ships;
extern unsigned int trade_ship_buf_size;
extern float next_event;

// COMBAT MODE STATE
extern C_PLAYER c_player;
extern C_UNIT *npc_units;
// Index of enemy ship in chunk buffer which is currently engaged in combat
// with player
extern unsigned int e_enemy_index;
extern unsigned int num_npc_units;
// Specifies width and height of game arena in tiles
extern ivec2 arena_dimensions;
