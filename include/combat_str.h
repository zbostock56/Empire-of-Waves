#ifndef __COMBAT_STR__
#define __COMBAT_STR__

#include <cglm/vec2.h>
#define PROJ_RAD (0.75)

typedef enum unit_type {
  ENEMY,
  ALLY
} UNIT_T;

typedef enum weapon_t {
  MELEE,
  RANGED
} WEAPON_T;

typedef struct projectile {
  vec2 pos;
  vec2 dir;
  float speed;
  UNIT_T target;
} PROJ;

#endif
