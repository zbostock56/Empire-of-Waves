#include <combat.h>
/*
                              COMBAT.c
  Implements much of the statemanagement functions that are used during combat
  mode such as switching to and from combat mode, and tracking the current
  win/loss conditions
*/

int to_combat_mode(unsigned int enemy_index) {
  mode = COMBAT;
  CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  E_ENEMY *enemy_ship = cur_chunk->enemies + enemy_index;

  // Initialize combat mode player
  c_player.weapon_type = RANGED;
  c_player.ammo = 5;
  c_player.max_health = 100.0;
  c_player.health = 100.0;
  c_player.speed = 1.0;
  c_player.proj_speed = 5.0;
  c_player.attack_active = 0.0;
  c_player.attack_cooldown = 0.0;
  c_player.fire_rate = 0.5;
  c_player.invuln_timer = 0.0;
  glm_vec2_zero(c_player.direction);
  glm_vec2_zero(c_player.coords);
  // Spawn player on left side of the arena
  c_player.direction[X] = 1.0;
  c_player.coords[X] = -arena_dimensions[X] / 4;

  // Initialize combat mode enemies
  e_enemy_index = enemy_index;
  num_npc_units = enemy_ship->crew_count;
  npc_units = malloc(sizeof(C_UNIT) * num_npc_units);
  if (npc_units == NULL) {
    fprintf(stderr, "combat.c: unable to allocate npc_units buffer\n");
    return -1;
  }
  projectiles = malloc(sizeof(PROJ) * PROJ_BUF_START_LEN);
  if (projectiles == NULL) {
    fprintf(stderr, "combat.c: unable to allocate projectiles buffer\n");
    return -1;
  }
  num_projectiles = 0;
  proj_buf_size = PROJ_BUF_START_LEN;

  for (unsigned int i = 0; i < num_npc_units; i++) {
    npc_units[i].type = ENEMY;
    int weapon_type = rand() % 100;
    npc_units[i].ammo = 0;
    if (weapon_type < MELEE_CUTOFF) {
      npc_units[i].weapon_type = MELEE;
    } else {
      npc_units[i].weapon_type = RANGED;
      npc_units[i].ammo = 5;
    }
    npc_units[i].speed = 0.5;
    npc_units[i].death_animation = -1.0;
    npc_units[i].attack_active = 0.0;
    npc_units[i].attack_cooldown = 0.0;
    npc_units[i].fire_rate = 1.0;
    glm_vec2_zero(npc_units[i].direction);
    glm_vec2_zero(npc_units[i].coords);
    // Spawn enemies in line on right side of the arena
    npc_units[i].direction[X] = -1.0;
    npc_units[i].coords[X] = arena_dimensions[X] / 4;
    npc_units[i].coords[Y] = ((arena_dimensions[Y] - 2) / 2) - (i * 2);
  }

  return 0;
}

void from_combat_mode() {
  mode = EXPLORATION;
  free(npc_units);
  free(projectiles);
  projectiles = NULL;
  npc_units = NULL;
  num_npc_units = 0;
  num_projectiles = 0;
  proj_buf_size = 0;
  CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  // Remove enemy ship from exploration mode
  cur_chunk->num_enemies--;
  unsigned int last_enemy_index = cur_chunk->num_enemies;
  // Swap last enemy ship with deleted enemy ship
  cur_chunk->enemies[e_enemy_index] = cur_chunk->enemies[last_enemy_index];
}

void update_combat_state() {
  if (mode != COMBAT) {
    return;
  }

  c_player.attack_cooldown = decrement_timer(c_player.attack_cooldown);
  c_player.attack_active = decrement_timer(c_player.attack_active);
  c_player.invuln_timer = decrement_timer(c_player.invuln_timer);

  for (unsigned int i = 0; i < num_npc_units; i++) {
    npc_units[i].attack_active = decrement_timer(npc_units[i].attack_active);
    npc_units[i].attack_cooldown = decrement_timer(npc_units[i].
                                                   attack_cooldown);
    if (npc_units[i].death_animation > 0.0) {
      // Npc currently in death animation
      npc_units[i].death_animation = decrement_timer(npc_units[i].
                                                     death_animation);
    } else if (npc_units[i].death_animation == 0.0) {
      // Death animation complete, npc should be deleted
      num_npc_units--;
      // move last unit in buffer to position of deleted unit
      npc_units[i] = npc_units[num_npc_units];
      i--;
    } else {
      // NPC still alive, has not been hit
      c_enemy_pathfind(npc_units + i, c_player.coords);
    }
  }

  if (num_npc_units == 0) {
    // Player wins
    from_combat_mode();
  } else if (c_player.health <= 0.0) {
    // Player loses
    from_combat_mode();
  }
}

float decrement_timer(float timer) {
  timer -= delta_time;
  if (timer < 0.0) {
    timer = 0.0;
  }
  return timer;
}

void update_projectiles() {
  if (mode != COMBAT) {
    return;
  }

  PROJ *cur_proj = NULL;
  vec2 movement = GLM_VEC2_ZERO_INIT;
  for (unsigned int i = 0; i < num_projectiles; i++) {
    cur_proj = projectiles + i;
    glm_vec2_scale(cur_proj->dir, delta_time * cur_proj->speed, movement);
    glm_vec2_add(movement, cur_proj->pos, cur_proj->pos);
  }
}

int spawn_projectile(vec2 pos, vec2 dir, float speed, UNIT_T target) {
  if (mode != COMBAT) {
    return 0;
  }

  PROJ *new_proj = projectiles + num_projectiles;
  glm_vec2_copy(pos, new_proj->pos);
  glm_vec2_copy(dir, new_proj->dir);
  new_proj->speed = speed;
  new_proj->target = target;
  num_projectiles++;
  if (num_projectiles == proj_buf_size) {
    int status = double_buffer((void **) &projectiles, &proj_buf_size,
                               sizeof(PROJ));
    if (status) {
      fprintf(stderr, "combat.c: Failed to reallocate projectile buffer\n");
      return -1;
    }
  }

  return 0;
}

void despawn_projectile(unsigned int index) {
  if (index >= num_projectiles) {
    return;
  }

  num_projectiles--;
  projectiles[index] = projectiles[num_projectiles];
}
