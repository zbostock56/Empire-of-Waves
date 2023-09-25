// EXPLORATION MODE STATE
extern E_PLAYER e_player;
CHUNK player_chunks[9];
TRADE_SHIP *trade_ships;
// Each trade ship contains a single loaded chunk
// Each chunk corresponds to the trade ship of the same
// index in trade_ships
CHUNK *trade_ship_chunks;
unsigned int num_trade_ships;
float next_event;

// COMBAT MODE STATE
C_PLAYER c_player;
C_UNIT *npc_units;
unsigned int num_npc_units;
