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
  c_player.speed = 15.0;
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
  num_npc_units = enemy_ship->crew_count + e_player.ship_mercenaries;
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

  for (int i = 0; i < enemy_ship->crew_count; i++) {
    npc_units[i].type = ENEMY;
    int weapon_type = rand() % 100;
    npc_units[i].ammo = 0;
    if (weapon_type < MELEE_CUTOFF) {
      npc_units[i].weapon_type = MELEE;
    } else {
      npc_units[i].weapon_type = RANGED;
      npc_units[i].ammo = 5;
    }
    float speed_mod = 1.0 * (rand() % 5);
    npc_units[i].speed = 10.0 + speed_mod;
    npc_units[i].death_animation = -1.0;
    npc_units[i].attack_active = 0.0;
    npc_units[i].attack_cooldown = 0.0;
    npc_units[i].fire_rate = 1.0;
    npc_units[i].max_life = 20.0;
    npc_units[i].knockback_counter = -1.0;
    npc_units[i].life = 20.0;
    glm_vec2_zero(npc_units[i].direction);
    glm_vec2_zero(npc_units[i].coords);
    // Spawn enemies in line on right side of the arena
    npc_units[i].direction[X] = -1.0;
    npc_units[i].coords[X] = arena_dimensions[X] / 4;
    npc_units[i].coords[Y] = ((arena_dimensions[Y] - 2) / 2) - (i * 2);
  }

  for (int i = enemy_ship->crew_count; i < num_npc_units; i++) {
    npc_units[i].type = ALLY;
    int weapon_type = rand() % 100;
    npc_units[i].ammo = 0;
    if (weapon_type < MELEE_CUTOFF) {
      npc_units[i].weapon_type = MELEE;
    } else {
      npc_units[i].weapon_type = RANGED;
      npc_units[i].ammo = 5;
    }
    float speed_mod = 1.0 * (rand() % 5);
    npc_units[i].speed = 10.0 + speed_mod;
    npc_units[i].death_animation = -1.0;
    npc_units[i].attack_active = 0.0;
    npc_units[i].attack_cooldown = 0.0;
    npc_units[i].fire_rate = 1.0;
    npc_units[i].max_life = 20.0;
    npc_units[i].knockback_counter = -1.0;
    npc_units[i].life = 20.0;
    glm_vec2_zero(npc_units[i].direction);
    glm_vec2_zero(npc_units[i].coords);
    // Spawn allies in line on left side of the arena
    npc_units[i].direction[X] = -1.0;
    npc_units[i].coords[X] = -arena_dimensions[X] / 4;
    npc_units[i].coords[Y] = ((arena_dimensions[Y] - 2) / 2) - (i * 2);
    fflush(stdout);
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
    if (npc_units[i].invuln_timer > 0.0) {
      npc_units[i].invuln_timer = decrement_timer(npc_units[i].invuln_timer);
    }
    if (npc_units[i].knockback_counter > 0.0) {
      npc_units[i].knockback_counter = decrement_timer(npc_units[i].
                                                       knockback_counter);
      knockback(npc_units+i);
      return;
    }
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
      float min_distance = FLT_MAX;
      int min_idx = 0;
      float distance;
      for (int j = 0; j < num_npc_units; j++) {
        if (npc_units[i].type == npc_units[j].type) {
          continue;
        }
        distance = glm_vec2_distance(npc_units[i].coords, npc_units[j].coords);
        if (distance < min_distance) {
          min_distance = distance;
          min_idx = j;
        }
      }
      // if it's an enemy unit, need to also consider Player.
      if (npc_units[i].type == ENEMY) {
        distance = glm_vec2_distance(npc_units[i].coords, c_player.coords);
        if (distance < min_distance) {
          min_distance = distance;
          min_idx = -1;
        }
      }

      if (min_idx == -1) {
        c_enemy_pathfind(npc_units + i, c_player.coords);
      } else if (num_npc_units) {
        c_enemy_pathfind(npc_units + i, npc_units[min_idx].coords);
      }
    }
  }

  unsigned int num_allies = 0;
  unsigned int num_enemies = 0;
  for (unsigned int i = 0; i < num_npc_units; i++) {
    if (npc_units[i].type == ENEMY) {
      num_enemies++;
    } else {
      num_allies++;
    }
  }

  if (num_enemies == 0) {
    // Player wins
    e_player.ship_mercenaries = num_allies;
    from_combat_mode();
  }
  if (c_player.health <= 0.0) {
    // Player loses
    e_player.ship_mercenaries = 0;
    from_combat_mode();
  }
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

/*
  Spawns a projectile in combat mode
  Args:
  - vec2 pos: starting position of the projectile
  - vec2 dir: direction of travel
  - float speed: speed of travel
  - UNIT_T target: Target team of projectile. If ALLY, projectile will only hit
    the player and mercenaries. If enemy, the projectile will only hit enemies.
  Returns:
  0 if successful, nonzero if failed due to buffer reallocation failure
*/
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

/*
  Spawns a melee attack from a unit in combat mode
  Args:
  - C_UNIT *unit: Combat mode unit from which to spawn the attack
*/
void npc_melee_attack(C_UNIT *unit) {
  unit->attack_cooldown = unit->fire_rate;
  unit->attack_active = 0.1;
}

void npc_ranged_attack(C_UNIT *unit) {
  unit->attack_cooldown = unit->fire_rate;
  unit->attack_active = 0.1;
  vec2 proj_coords = { 0.0, 0.0 };
  vec2 proj_dir = { 0.0, 0.0 };
  glm_vec2_scale(unit->coords, T_WIDTH, proj_coords);
  proj_coords[1] += T_WIDTH;
  glm_vec2_copy(unit->direction, proj_dir);
  proj_dir[1] -= T_WIDTH;
  if (unit->type == ENEMY) {
    spawn_projectile(proj_coords, proj_dir, 3.0,
                    ALLY);
  } else {
    spawn_projectile(proj_coords, proj_dir, 3.0,
                    ENEMY);
  }
}

/*
  Applies a Knockback behavior to a unit
  Args:
  - C_UNIT *unit: Combat mode unit to get knockbacked
*/
void knockback(C_UNIT *unit) {
  vec2 movement = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(unit->direction, delta_time * unit->speed, movement);
  glm_vec2_sub(unit->coords, movement, unit->coords);
}

/*
  Perform surrender actions
  Assumption: Only possible when in combat mode.
*/
void perform_surrender() {
  if (mode == EXPLORATION) {
    return;
  }
  
  from_combat_mode();

  // If has merceneries assigned, get rid of them
  if (e_player.ship_mercenaries > 0) {
    e_player.ship_mercenaries = 0;
  }

  // If player has >0 item, get rid of one of them randomly. Should also work o
  int item_indices[MAX_PLAYER_INV_SIZE];
  int num_item = 0;
  for (int i = 0; i < MAX_PLAYER_INV_SIZE; i++) {
    if (get_player_inventory_slot_by_index(i)->item_id != EMPTY) {
      item_indices[num_item++] = i;
    }
  }
  if (num_item > 0) {
    
    int lost_item_idx = item_indices[rand() % num_item];
    I_SLOT *lost_item = get_player_inventory_slot_by_index(lost_item_idx);
    printf("ITEM: %s\n", get_item_name_by_ID(lost_item->item_id));   
    //ui this lost_item was lost.
    
  }
}