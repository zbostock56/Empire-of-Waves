#ifndef __PLAYER_STR_H__
#define __PLAYER_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>
#include <combat_str.h>
#include <items.h>
#include <container_str.h>
/*
                                PLAYER_STR.h
Describes the struct representing player state in exploration mode, and the
struct representing player state in combat mode. Can be included by any file
which reads/manipulates player state.
*/

#define MAX_PLAYER_INV_SIZE (16)
#define REP_MIN (-100000.0)
#define REP_MAX (100.0)

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
  int moving;
  int ship_moving;
  I_SLOT inventory[MAX_PLAYER_INV_SIZE];
  unsigned int resources[NUM_RESOURCES];
  unsigned int money;
  unsigned int total_mercenaries;
  /* Number of mercenaries assigned to */
  /* player's ship */
  unsigned int ship_mercenaries;
  float max_health;
  float health;
  /* Value between 0.0 and 100.0 */
  /* Once value gets below 10.0, the player begins to lose */
  /* health. */
  /* In the same vein, when the player's hunger is above 90.0 */
  /* the player begins to gain health. */
  float hunger;
  float speed;
  // Number representing reputation of player amongst merchants. If negative,
  // there is a possibility for the player islant to be invaded
  float reputation;
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
  float proj_speed;
  // Indicates the current cool down of a players active hitbox in seconds.
  // When non-zero, the hitbox is currently active. When 0, the hitbox is not
  // active
  float attack_active;
  // Indicates current cool down of players attack in seconds. Once at 0, the
  // player can attack again
  float attack_cooldown;
  // Value that attack_timer is set to once an attack is fired off
  float fire_rate;
  float invuln_timer;
} C_PLAYER;

typedef struct temp_item {
  I_SLOT slot;
} TEMP_ITEM;

#endif
