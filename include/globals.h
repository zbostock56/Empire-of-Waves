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
extern TRADE_SHIP *trade_ships;
// Each trade ship contains a single loaded chunk
// Each chunk corresponds to the trade ship of the same
// index in trade_ships
extern CHUNK *trade_ship_chunks;
extern unsigned int num_trade_ships;
extern float next_event;

// COMBAT MODE STATE
extern C_PLAYER c_player;
extern C_UNIT *npc_units;
extern unsigned int num_npc_units;

// DIALOG STATE
extern int isTalking;
