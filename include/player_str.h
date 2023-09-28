#include <cglm/vec2.h>
#include <cglm/ivec2.h>
/*
                                PLAYER_STR.h
Describes the struct representing player state in exploration mode, and the
struct representing player state in combat mode. Can be included by any file
which reads/manipulates player state.
*/

#define INV_SIZE (10)

#ifndef NUM_RESOURCES
#define NUM_RESOURCES (1) // Also defined in items.h
#endif

/*
  Enum describing weapon types
  Wrapped in def guards because WEAPON_T is also defined in enemy_str.h
*/
#ifndef WT
#define WT
typedef enum weapon_t {
  MELEE,
  RANGED
} WEAPON_T;
#endif

typedef struct inventory_slot {
  unsigned int item_id;
  unsigned int quantity;
} I_SLOT;

/*
  Player state in exploration mode
*/
typedef struct exploration_player {
  ivec2 chunk;
  ivec2 ship_chunk;
  vec2 coords;
  vec2 ship_coords;
  vec2 direction;
  vec2 ship_direction;
  int embarked;
  I_SLOT inventory[INV_SIZE];
  unsigned int resources[NUM_RESOURCES];
  unsigned int money;
  unsigned int total_mercenaries;
  float max_health;
  float health;
  float speed;
} E_PLAYER;

/*
  Player state in combat mode
*/
typedef struct combat_player {
  vec2 coords;
  vec2 direction;
  WEAPON_T weapon_type;
  unsigned int ammo;
  float max_health;
  float health;
  float speed;
  float firerate;
} C_PLAYER;
