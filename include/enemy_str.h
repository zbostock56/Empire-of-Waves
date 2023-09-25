#include <cglm/vec2.h>
#include <cglm/ivec2.h>
/*
                                   ENEMY_STR.h
Describes the struct representing an enemy ship in exploration mode and the
struct representing an enemy unit in combat mode. Can be included by any file
which reads/manipulates enemy state.
*/

/*
  Enum describing weapon types
  Also defined in player_str.h
*/
#ifndef WT
#define WT
typedef enum weapon_t {
  MELEE,
  RANGED
} WEAPON_T;
#endif

typedef enum unit_type {
  ENEMY,
  ALLY
} UNIT_T;

/*
  State of enemy ship in exploration mode
*/
typedef struct exploration_enemy {
  ivec2 chunk;
  vec2 coords;
  vec2 direction;
  unsigned int crew_count;
  float speed;
} E_ENEMY;

/*
  State of combat mode npc unit
*/
typedef struct combat_unit {
  vec2 coords;
  vec2 direction;
  UNIT_T type;
  WEAPON_T weapon_type;
  unsigned int ammo;
  float speed;
  float fire_rate;
} C_UNIT;
