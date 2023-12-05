#include <combat.h>
/*
                              COMBAT.c
  Implements much of the statemanagement functions that are used during combat
  mode such as switching to and from combat mode, and tracking the current
  win/loss conditions
*/

int to_combat_mode(unsigned int enemy_index) {
  if (mode == COMBAT) {
    return 0;
  }

  mode = COMBAT;
  CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  E_ENEMY *enemy_ship = cur_chunk->enemies + enemy_index;

  // Initialize combat mode player
  c_player.weapon_type = equipment.weapon_type;
  c_player.ammo = 5;
  // c_player.max_health = 100.0;
  // c_player.health = 100.0;
  // c_player.speed = 15.0;
  c_player.proj_speed = 5.0;
  c_player.attack_active = 0.0;
  c_player.attack_cooldown = 0.0;
  // c_player.fire_rate = 0.5;
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

  loot = malloc(sizeof(L_UNIT) * enemy_ship->crew_count);
  if (loot == NULL) {
    fprintf(stderr, "combat.c: unable to allocate loot buffer\n");
    return -1;
  }
  loot_buf_len = enemy_ship->crew_count;
  for (int i = 0; i < loot_buf_len; i++) {
    init_container(&loot[i].inv, ENEMY_INV_SIZE);
    loot[i].inv.items[0].item_id = COPPER_COIN;
    loot[i].inv.items[0].quantity = (rand() % 10) + 1;
  }
  num_loot = 0;

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
    npc_units[i].invuln_timer = 0.0;
    npc_units[i].max_life = 20.0;
    npc_units[i].knockback_counter = -1.0;
    npc_units[i].life = 20.0;
    npc_units[i].moving = 0;
    glm_vec2_zero(npc_units[i].direction);
    glm_vec2_zero(npc_units[i].coords);
    // Spawn enemies in line on right side of the arena
    npc_units[i].direction[X] = -1.0;
    npc_units[i].coords[X] = (arena_dimensions[X] / 4) - (i / 5);
    npc_units[i].coords[Y] = ((arena_dimensions[Y] / 2) - 5) + (i % 5);
  }

  for (int i = enemy_ship->crew_count; i < num_npc_units; i++) {
    int ally_index = i - enemy_ship->crew_count;
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
    npc_units[i].direction[X] = 1.0;
    npc_units[i].coords[X] = (-arena_dimensions[X] / 4) + (ally_index / 5);
    npc_units[i].coords[Y] = (arena_dimensions[Y] / 2 - 3) - (ally_index % 5);
    fflush(stdout);
  }

  close_all_menus();

  get_ui_component_by_ID(SURRENDER_BUTTON)->enabled = 1;

  return 0;
}

void from_combat_mode() {
  if (mode != COMBAT) {
    return;
  }

  mode = EXPLORATION;
  for (unsigned int i = 0; i < loot_buf_len; i++) {
    free_container(&loot[i].inv);
  }
  free(loot);
  free(npc_units);
  free(projectiles);
  projectiles = NULL;
  npc_units = NULL;
  loot_buf_len = 0;
  num_loot = 0;
  num_npc_units = 0;
  num_projectiles = 0;
  proj_buf_size = 0;
  CHUNK *cur_chunk = chunk_buffer + player_chunks[PLAYER_CHUNK];
  // Remove enemy ship from exploration mode
  cur_chunk->num_enemies--;
  unsigned int last_enemy_index = cur_chunk->num_enemies;
  // Swap last enemy ship with deleted enemy ship
  cur_chunk->enemies[e_enemy_index] = cur_chunk->enemies[last_enemy_index];
  get_ui_component_by_ID(COMBAT_INFO_BAR)->enabled = 0;
  close_container();
  get_ui_component_by_ID(SURRENDER_BUTTON)->enabled = 0;

  close_all_menus();
}

void update_combat_state() {
  if (mode != COMBAT) {
    return;
  }
  /* Update item type being changed during combat */
  c_player.weapon_type = equipment.weapon_type;
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
      if (npc_units[i].type == ENEMY) {
        glm_vec2_copy(npc_units[i].coords, loot[num_loot].coords);
        num_loot++;
      }

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

  // Remove all empty loot boxes
  if (!container_menu_open) {
    for (int i = 0; i < num_loot; i++) {
      int empty = 1;
      for (int j = 0; j < loot[i].inv.capacity; j++) {
        if (loot[i].inv.items[j].item_id != EMPTY) {
          empty = 0;
          break;
        }
      }

      if (empty) {
        L_UNIT temp = loot[i];
        num_loot--;
        loot[i] = loot[num_loot];
        loot[num_loot] = temp;
        i--;
      }
    }
  }

  unsigned int num_allies = 0;
  unsigned int num_enemies = 0;
  for (unsigned int i = 0; i < num_npc_units; i++) {
    if (npc_units[i].type == ALLY) {
      num_allies++;
    } else {
      num_enemies++;
    }
  }

  if (!num_enemies) {
    get_ui_component_by_ID(COMBAT_INFO_BAR)->enabled = 1;
    get_ui_component_by_ID(SURRENDER_BUTTON)->enabled = 0;
  }

  e_player.ship_mercenaries = num_allies;
  if (c_player.health <= 0.0) {
    // Player loses
    e_player.ship_mercenaries = 0;
    from_combat_mode();
    respawn_player();
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

void leave_combat() {
  unsigned int num_enemies = 0;
  for (unsigned int i = 0; i < num_npc_units; i++) {
    if (npc_units[i].type == ENEMY) {
      num_enemies++;
    }
  }

  if (num_enemies) {
    return;
  }

  from_combat_mode();
}
/*
  Perform surrender actions
  Assumption: Only possible when in combat mode.
*/
void perform_surrender() {
  if (mode == EXPLORATION) {
    return;
  }
  close_surrender_prompt();
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
    I_SLOT *last_item = get_player_inventory_slot_by_index(item_indices[num_item-1]);
    snprintf(lost_item_prompt_buffer, PROMPT_BUFFER_MAX, " %s got lost. ", get_item_name_by_ID(lost_item->item_id));
    set_prompt(lost_item_prompt_buffer);
    lost_item->item_id = last_item->item_id;
    lost_item->quantity = last_item->quantity;
    last_item->item_id = EMPTY;
    last_item->quantity = EMPTY;

  }
}

void init_surrender_ui() {
  UI_COMPONENT * surrender_button = get_ui_component_by_ID(SURRENDER_BUTTON);

  vec2 button_pos = { -0.95, -0.5 };
  init_menu(
    button_pos, // position
    open_surrender_prompt, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Surrender", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.75, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    surrender_button // dest
  );

  UI_COMPONENT * surrender_prompt = get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT);
  vec2 prompt_pos = {0, 0.15};
  init_menu(
    prompt_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Captain, your crew is counting on you.", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.75, // text_scale
    1.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    surrender_prompt // dest
  );

  UI_COMPONENT *surrender_prompt_2 = get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_2);
  vec2 prompt_pos_2 = {0.0, 0.0};
  init_menu(
    prompt_pos_2, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Surrendering means losing valuable items and loyal mercenaries.", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.75, // text_scale
    1.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    surrender_prompt_2 // dest
  );

  UI_COMPONENT *surrender_prompt_3 = get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_3);
  vec2 prompt_pos_3 = {0.0, -0.15};
  init_menu(
    prompt_pos_3, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Are you sure you want to proceed?", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.75, // text_scale
    1.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    surrender_prompt_3 // dest
  );

  UI_COMPONENT *prompt_yes = get_ui_component_by_ID(SURRENDER_CONFIRMATION_YES);
  UI_COMPONENT *prompt_no = get_ui_component_by_ID(SURRENDER_CONFIRMATION_NO);
  vec2 yes_pos = {0.2, -0.3};
  vec2 no_pos = {-0.2, -0.3};
  init_menu(
    yes_pos, // position
    perform_surrender, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Yes, I surrender.", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    prompt_yes // dest
  );

  init_menu(
    no_pos, // position
    close_surrender_prompt, // on_click
    NULL, // on_hover
    NULL, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "No, I'll stay and fight!", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    prompt_no // dest
  );
}

void open_surrender_prompt() {
  surrender_menu_open = 1;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT)->enabled = 1;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_2)->enabled = 1;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_3)->enabled = 1;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_YES)->enabled = 1;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_NO)->enabled = 1;
}

void close_surrender_prompt() {
  surrender_menu_open = 0;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT)->enabled = 0;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_2)->enabled = 0;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_PROMPT_3)->enabled = 0;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_YES)->enabled = 0;
  get_ui_component_by_ID(SURRENDER_CONFIRMATION_NO)->enabled = 0;
}
