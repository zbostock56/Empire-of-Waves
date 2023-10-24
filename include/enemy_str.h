#ifndef __ENEMY_STR_H__
#define __ENEMY_STR_H__

#include <cglm/vec2.h>
#include <cglm/ivec2.h>
#include <combat_str.h>
/*
                                   ENEMY_STR.h
Describes the struct representing an enemy ship in exploration mode and the
struct representing an enemy unit in combat mode. Can be included by any file
which reads/manipulates enemy state.
*/

/* DEFINES */
/* Specifies the max number of enemies per chunk */
#define MAX_ENEMIES (3)
#define CHUNKS_SIMULATED (9)
#define STEER_SPEED (3.0)

/*
  State of enemy ship in exploration mode
*/
typedef struct exploration_enemy {
  ivec2 chunk;
  vec2 coords;
  vec2 direction;
  unsigned int crew_count;
  float speed;
  bool on_path;
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
  // Indicates the current step in the death animation of the enemy in the case
  // it collides with a player hitbox. If -1.0, the enemy has not yet been hit.
  // If > 0.0, the death animation is currently active, and if 0.0, the enemy
  // is ready for deletion
  float death_animation;
  // Indicates the current cool down of an enemy's active hitbox in seconds.
  // When non-zero, the hitbox is currently active. When 0, the hitbox is not
  // active
  float attack_active;
  // Indicates current cool down of an ememy's attack in seconds. Once at 0,
  // the enemy can attack again
  float attack_cooldown;
  // Value that attack_timer is set to once an attack is fired off
  float fire_rate;
} C_UNIT;

#endif
