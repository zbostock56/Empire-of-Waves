#include <render.h>

// TODO Error checking for shader, model, texture, font, etc.. importing
// - Program will crash if assets do not exist

void init_scene() {
  // TEST MODELS
  //  unsigned char ocean_buffer[3] = { 3, 157, 252 };
  //  ocean_texture = texture_from_buffer(ocean_buffer, 1, 1, GL_RGB);
  // END TEST

  // Initialize offscreen framebuffer
  entity_framebuffer = framebuffer_init();

  // Create Resource Textures
  resource_textures[resource_to_buffer(INVALID_REC)] =
     gen_texture("assets/resources/invalid.png");
  resource_textures[resource_to_buffer(REC_EMPTY)] =
     gen_texture("assets/resources/invalid.png");
  resource_textures[resource_to_buffer(REC_CITRUS)] =
     gen_texture("assets/resources/citrus.png");
  resource_textures[resource_to_buffer(REC_LIFE_POTION)] =
     gen_texture("assets/resources/life_potion.png");
  resource_textures[resource_to_buffer(REC_FIRERATE_POTION)] =
     gen_texture("assets/resources/firerate_potion.png");
  resource_textures[resource_to_buffer(REC_SPEED_POTION)] =
     gen_texture("assets/resources/speed_potion.png");
  resource_textures[resource_to_buffer(REC_KNIFE)] =
     gen_texture("assets/resources/knife.png");
  resource_textures[resource_to_buffer(REC_SWORD)] =
     gen_texture("assets/resources/sword.png");
  resource_textures[resource_to_buffer(REC_AXE)] =
     gen_texture("assets/resources/axe.png");
  resource_textures[resource_to_buffer(REC_BOW)] =
     gen_texture("assets/resources/bow.png");
  resource_textures[resource_to_buffer(REC_CROSSBOW)] =
     gen_texture("assets/resources/crossbow.png");
  resource_textures[resource_to_buffer(REC_FLINTLOCK)] =
     gen_texture("assets/resources/flintlock.png");
  resource_textures[resource_to_buffer(REC_CLOTH_ARMOR)] =
     gen_texture("assets/resources/cloth_armor.png");
  resource_textures[resource_to_buffer(REC_LEATHER_ARMOR)] =
     gen_texture("assets/resources/leather_armor.png");
  resource_textures[resource_to_buffer(REC_LIGHT_ARMOR)] =
     gen_texture("assets/resources/light_armor.png");
  resource_textures[resource_to_buffer(REC_MEDIUM_ARMOR)] =
     gen_texture("assets/resources/medium_armor.png");
  resource_textures[resource_to_buffer(REC_HEAVY_ARMOR)] =
     gen_texture("assets/resources/heavy_armor.png");
  resource_textures[resource_to_buffer(REC_PLATE_ARMOR)] =
     gen_texture("assets/resources/plate_armor.png");
  resource_textures[resource_to_buffer(REC_GOLD_COIN)] =
     gen_texture("assets/resources/gold_coin.png");
  resource_textures[resource_to_buffer(REC_SILVER_COIN)] =
     gen_texture("assets/resources/silver_coin.png");
  resource_textures[resource_to_buffer(REC_COPPER_COIN)] =
     gen_texture("assets/resources/copper_coin.png");
  resource_textures[resource_to_buffer(GRAIN)] =
     gen_texture("assets/resources/grain.png");
  resource_textures[resource_to_buffer(COTTON)] =
     gen_texture("assets/resources/cotton.png");
  resource_textures[resource_to_buffer(WOOL)] =
     gen_texture("assets/resources/wool.png");
  resource_textures[resource_to_buffer(DYES)] =
     gen_texture("assets/resources/dyes.png");
  resource_textures[resource_to_buffer(SUGAR)] =
     gen_texture("assets/resources/sugar.png");
  resource_textures[resource_to_buffer(LEATHER)] =
     gen_texture("assets/resources/leather.png");
  resource_textures[resource_to_buffer(CHEESE)] =
     gen_texture("assets/resources/cheese.png");
  resource_textures[resource_to_buffer(IRON_ORE)] =
     gen_texture("assets/resources/iron_ore.png");
  resource_textures[resource_to_buffer(WINE)] =
     gen_texture("assets/resources/wine.png");
  resource_textures[resource_to_buffer(COPPER_ORE)] =
     gen_texture("assets/resources/copper_ore.png");
  resource_textures[resource_to_buffer(SPICE)] =
     gen_texture("assets/resources/spice.png");
  resource_textures[resource_to_buffer(HERB)] =
     gen_texture("assets/resources/herb.png");
  resource_textures[resource_to_buffer(TEA)] =
     gen_texture("assets/resources/tea.png");
  resource_textures[resource_to_buffer(SILVER_ORE)] =
     gen_texture("assets/resources/silver_ore.png");
  resource_textures[resource_to_buffer(PORCELAIN)] =
     gen_texture("assets/resources/porcelain.png");
  resource_textures[resource_to_buffer(SILK)] =
     gen_texture("assets/resources/silk.png");
  resource_textures[resource_to_buffer(PEARL)] =
     gen_texture("assets/resources/pearl.png");
  resource_textures[resource_to_buffer(GOLD_ORE)] =
     gen_texture("assets/resources/gold_ore.png");
  resource_textures[resource_to_buffer(SAFFRON)] =
     gen_texture("assets/resources/saffron.png");
  resource_textures[resource_to_buffer(AMBERGRIS)] =
     gen_texture("assets/resources/ambergris.png");

  hunger_bar_textures[0] = gen_texture("assets/hunger_bars/hunger_bar.png");
  hunger_bar_textures[1] = gen_texture("assets/hunger_bars/hunger_bar_1.png");
  hunger_bar_textures[2] = gen_texture("assets/hunger_bars/hunger_bar_2.png");
  hunger_bar_textures[3] = gen_texture("assets/hunger_bars/hunger_bar_3.png");
  hunger_bar_textures[4] = gen_texture("assets/hunger_bars/hunger_bar_4.png");
  hunger_bar_textures[5] = gen_texture("assets/hunger_bars/hunger_bar_5.png");
  hunger_bar_textures[6] = gen_texture("assets/hunger_bars/hunger_bar_6.png");
  hunger_bar_textures[7] = gen_texture("assets/hunger_bars/hunger_bar_7.png");
  hunger_bar_textures[8] = gen_texture("assets/hunger_bars/hunger_bar_8.png");
  hunger_bar_textures[9] = gen_texture("assets/hunger_bars/hunger_bar_9.png");
  hunger_bar_textures[10] = gen_texture("assets/hunger_bars/hunger_bar_10.png");

  health_bar_textures[0] = gen_texture("assets/health_bars/health_bar.png");
  health_bar_textures[1] = gen_texture("assets/health_bars/health_bar_1.png");
  health_bar_textures[2] = gen_texture("assets/health_bars/health_bar_2.png");
  health_bar_textures[3] = gen_texture("assets/health_bars/health_bar_3.png");
  health_bar_textures[4] = gen_texture("assets/health_bars/health_bar_4.png");
  health_bar_textures[5] = gen_texture("assets/health_bars/health_bar_5.png");
  health_bar_textures[6] = gen_texture("assets/health_bars/health_bar_6.png");
  health_bar_textures[7] = gen_texture("assets/health_bars/health_bar_7.png");
  health_bar_textures[8] = gen_texture("assets/health_bars/health_bar_8.png");
  health_bar_textures[9] = gen_texture("assets/health_bars/health_bar_9.png");
  health_bar_textures[10] = gen_texture("assets/health_bars/health_bar_10.png");

  bullet_texture = gen_texture("assets/bullet.png");

  // Initialize shaders
  std_shader = shader_init(vertex_shader, fragment_shader_texture);
  color_shader = shader_init(vertex_shader, fragment_shader_color);
  pixel_shader = shader_init(vertex_shader, fragment_shader_pixelated);
  text_shader = shader_init(vertex_shader, fragment_shader_text);
  ripple_shader = shader_init(vertex_shader, fragment_shader_ripple);
  chunk_wave_shader = shader_init(vertex_shader, fragment_shader_wave);
  chunk_rain_shader = shader_init(vertex_shader, fragment_shader_rain);
  island_shader = shader_init(vertex_shader, fragment_shader_island);
  weather_shader = shader_init(vertex_shader, fragment_shader_weather);

  // Initialize models
  //player = load_model("assets/player.bin", "assets/3A.png");
  player[STILL] = load_model("assets/player_still.bin", "assets/3A.png");
  player[WALK_1] = load_model("assets/player_walk_1.bin", "assets/3A.png");
  player[WALK_2] = load_model("assets/player_walk_2.bin", "assets/3A.png");
  player[WALK_3] = load_model("assets/player_walk_3.bin", "assets/3A.png");
  mercenary[STILL] = load_model("assets/player_still.bin", "assets/1A.png");
  mercenary[WALK_1] = load_model("assets/player_walk_1.bin", "assets/1A.png");
  mercenary[WALK_2] = load_model("assets/player_walk_2.bin", "assets/1A.png");
  mercenary[WALK_3] = load_model("assets/player_walk_3.bin", "assets/1A.png");
  enemy[STILL] = load_model("assets/enemy_still.bin", "assets/2A.png");
  enemy[WALK_1] = load_model("assets/enemy_walk_1.bin", "assets/2A.png");
  enemy[WALK_2] = load_model("assets/enemy_walk_2.bin", "assets/2A.png");
  enemy[WALK_3] = load_model("assets/enemy_walk_3.bin", "assets/2A.png");
  merchant = load_model("assets/merchant.bin", "assets/2A.png");
  player_ship = load_model("assets/player_ship.bin", "assets/1A.png");
  enemy_ship = load_model("assets/enemy_ship.bin", "assets/1B.png");
  trade_ship = load_model("assets/trade_ship.bin", "assets/2A.png");
  house = load_model("assets/quad.bin", "assets/House.png");
  chest = load_model("assets/chest.bin", "assets/1A.png");
  quad = load_model("assets/quad.bin", NULL);
  circle = load_model("assets/circle.bin", NULL);
  char default_path[50] = "assets/Dinklebitmap/x.bin";
  char lowercase_path[50] = "assets/Dinklebitmap/x_lower.bin";
  for (char cur = ' '; cur <= '~'; cur++) {
    int index = cur - ' ';
    if (cur == '/') {
      load_character("assets/Dinklebitmap/fs.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '$') {
      load_character("assets/Dinklebitmap/dollar.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '!') {
      load_character("assets/Dinklebitmap/e_mark.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '#') {
      load_character("assets/Dinklebitmap/pound.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '%') {
      load_character("assets/Dinklebitmap/percent.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '&') {
      load_character("assets/Dinklebitmap/amper.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '\'') {
      load_character("assets/Dinklebitmap/s_quote.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '(') {
      load_character("assets/Dinklebitmap/l_paren.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ')') {
      load_character("assets/Dinklebitmap/r_paren.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ',') {
      load_character("assets/Dinklebitmap/comma.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '-') {
      load_character("assets/Dinklebitmap/minus.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '+') {
      load_character("assets/Dinklebitmap/plus.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ':') {
      load_character("assets/Dinklebitmap/colon.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ';') {
      load_character("assets/Dinklebitmap/s_colon.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '<') {
      load_character("assets/Dinklebitmap/lt.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '=') {
      load_character("assets/Dinklebitmap/eq.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '>') {
      load_character("assets/Dinklebitmap/gt.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '?') {
      load_character("assets/Dinklebitmap/q_mark.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '@') {
      load_character("assets/Dinklebitmap/at.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '[') {
      load_character("assets/Dinklebitmap/l_brac.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ']') {
      load_character("assets/Dinklebitmap/r_brac.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '^') {
      load_character("assets/Dinklebitmap/carrot.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '_') {
      load_character("assets/Dinklebitmap/u_score.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '`') {
      load_character("assets/Dinklebitmap/grave.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '{') {
      load_character("assets/Dinklebitmap/lc_brac.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '|') {
      load_character("assets/Dinklebitmap/pipe.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '}') {
      load_character("assets/Dinklebitmap/rc_brac.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '~') {
      load_character("assets/Dinklebitmap/tilde.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == ' ') {
      load_character("assets/Dinklebitmap/space.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '\\') {
      load_character("assets/Dinklebitmap/bs.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '.') {
      load_character("assets/Dinklebitmap/period.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '"') {
      load_character("assets/Dinklebitmap/quote.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur == '*') {
      load_character("assets/Dinklebitmap/asterisk.bin",
                     "assets/Dinklebitmap/font.png", font + index);
    } else if (cur >= 'a' && cur <= 'z') {
      lowercase_path[20] = cur;
      load_character(lowercase_path, "assets/Dinklebitmap/font.png",
                     font + index);
    } else {
      default_path[20] = cur;
      load_character(default_path, "assets/Dinklebitmap/font.png",
                     font + index);
    }
  }

  // Setup perspective matrices
  glm_ortho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), ((float) RES_X) / ((float) RES_Y), 0.1,
                  100.0, persp_proj);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cleanup_scene() {
  for (int i = 0; i < NUM_PLAYER_FRAMES; i++) {
    free_model(player[i]);
  }
  for (int i = 0; i < NUM_PLAYER_FRAMES; i++) {
    free_model(mercenary[i]);
  }
  for (int i = 0; i < NUM_ENEMY_FRAMES; i++) {
    free_model(enemy[i]);
  }
  free_model(merchant);
  free_model(player_ship);
  free_model(enemy_ship);
  free_model(trade_ship);
  free_model(quad);
  for (int i = 0; i < FONT_LEN; i++) {
    free_model(font[i].model);
  }
}

void render_scene(GLFWwindow *window) {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;

  calc_screen_scale();

  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (int i = 0; i < NUM_COMPONENTS; i++) {
    if (ui_tab[i].enabled) {
      render_ui(ui_tab + i);
    }
  }

  render_player();
  if (mode == EXPLORATION) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        ivec2 chunk = {
          e_player.ship_chunk[0] - 2 + j,
          e_player.ship_chunk[1] - 2 + i
        };
        render_chunk(chunk);
      }
      if (!merchant_dialog_enabled) {
        render_player_health_bar();
        render_hunger_bar();
      }
    }

    render_player_ship();

    for (unsigned int i = 0; i < num_trade_ships; i++) {
      render_trade_ship(trade_ships + i, trade_ships[i].cur_chunk_index);
    }

    for (int i = 0; i < 9; i++) {
      CHUNK *cur_chunk = chunk_buffer + player_chunks[i];
      for (int j = 0; j < cur_chunk->num_enemies; j++) {
        render_enemy_ship(cur_chunk->enemies + j, player_chunks[i]);
      }
      for (int j = 0; j < cur_chunk->num_islands; j++) {
        if (cur_chunk->islands[j].has_merchant) {
          render_merchant(&cur_chunk->islands[j].merchant);
        }
        render_island(cur_chunk->islands + j);
        ISLAND *cur_island = cur_chunk->islands + j;
        /* Render the resource icons onto the islands */
        for (int k = 0; k < cur_island->num_items; k++) {
          if (cur_island->item_tiles[k].resource != INVALID_REC) {
            render_resource(cur_island->item_tiles[k].position,
                            cur_island,
                            cur_island->item_tiles[k].resource);
          }
        }
      }
    }

    // Render weather
    if (weather == FOG) {
      render_weather();
    }
  } else {
    if (npc_units) {
      for (int i = 0; i < num_npc_units; i++) {
        render_unit(npc_units + i);
        /*if (npc_units[i].attack_active) {
          vec2 hitbox_pos = GLM_VEC2_ZERO_INIT;
          vec2 hitbox_offset = GLM_VEC2_ZERO_INIT;
          glm_vec2_scale(npc_units[i].coords, T_WIDTH, hitbox_pos);
          glm_vec2_scale_as(npc_units[i].direction, T_WIDTH, hitbox_offset);
          hitbox_offset[1] += T_WIDTH;
          glm_vec2_add(hitbox_pos, hitbox_offset, hitbox_pos);
          render_hitbox(hitbox_pos, 1.0);
        }*/

        /*vec2 hitbox_pos = GLM_VEC2_ZERO_INIT;
        glm_vec2_scale(npc_units[i].coords, T_WIDTH, hitbox_pos);
        hitbox_pos[1] += T_WIDTH;
        render_hitbox(hitbox_pos, 1.0);*/
        vec2 health_bar_position = GLM_VEC2_ZERO_INIT;
        glm_vec2_scale(npc_units[i].coords, T_WIDTH, health_bar_position);
        health_bar_position[1]+= 2.7*T_WIDTH;
        render_health_bar_filled(health_bar_position, npc_units[i].max_life, npc_units[i].life);
        render_health_bar_background(health_bar_position);
        render_player_health_bar_combat();
      }
    }

    for (int i = 0; i < num_loot; i++) {
      render_loot(loot + i);
    }

    render_arena();

    if (c_player.attack_active) {
      vec2 hitbox_pos = GLM_VEC2_ZERO_INIT;
      vec2 hitbox_offset = GLM_VEC2_ZERO_INIT;
      glm_vec2_scale(c_player.coords, T_WIDTH, hitbox_pos);
      glm_vec2_scale_as(c_player.direction, T_WIDTH, hitbox_offset);
      hitbox_offset[1] += T_WIDTH;
      glm_vec2_add(hitbox_pos, hitbox_offset, hitbox_pos);
      render_hitbox(hitbox_pos, PROJ_RAD);
    }

    PROJ *cur_proj = NULL;
    for (unsigned int i = 0; i < num_projectiles; i++) {
      cur_proj = projectiles + i;
      render_hitbox(cur_proj->pos, PROJ_RAD);
    }
  }
  update_status_bar();
  update_status_menu();
  // Schedule trade route prompt delay
  if (time_schdule_trade_toute_prompt < 0) {
    dialog.ui_text_schedule_trade_route_prompt->enabled = 0;
    time_schdule_trade_toute_prompt = 2.0;
  } else {
    time_schdule_trade_toute_prompt -= delta_time;
  }
  // Trade prompt delay
  if (time_trade_event_prompt < 0) {
    trade.ui_text_event_prompt->enabled = 0;
    time_trade_event_prompt = 2.0;
  } else {
    time_trade_event_prompt -= delta_time;
  }
  // Inventory prompt delay
  if (time_inventory_event_prompt < 0) {
    inventory.ui_text_event_prompt->enabled = 0;
    time_inventory_event_prompt = 2.0;
  } else {
    time_inventory_event_prompt -= delta_time;
  }

  /*
  vec2 world_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(e_player.ship_chunk, e_player.ship_coords, world_coords);
  printf("world: { %f, %f }\nchunk: { %d, %d }\nchunk_coords: { %f, %f }\n\n",
         world_coords[0], world_coords[1],
         e_player.ship_chunk[0], e_player.ship_chunk[1],
         e_player.ship_coords[0], e_player.ship_coords[1]);
  */
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void render_player_ship() {
  mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 player_rot = GLM_MAT4_IDENTITY_INIT;
  vec3 player_dir = { e_player.ship_direction[0],
                      e_player.ship_direction[1], 0.0f };
  calc_rot_mat(player_dir, player_rot);
  glm_scale_uni(fbo_model_mat, 0.5);
  glm_rotate_x(fbo_model_mat, glm_rad(-25.0), fbo_model_mat);
  glm_mat4_mul(fbo_model_mat, player_rot, fbo_model_mat);

  mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate_z(fbo_view_mat, -3.0);

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 depth_offset = { 0.0, 0.0, SHIP_DEPTH };
  glm_translate(model_mat, depth_offset);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  if (!e_player.embarked) {
    /*
      Restricts the ship to stay still when the player moves off board.
    */
    vec3 world_coords = GLM_VEC3_ZERO_INIT;
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords, world_coords);
    glm_translate(model_mat, world_coords);

    vec3 player_world_coords = GLM_VEC3_ZERO_INIT;
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
    glm_vec3_negate(player_world_coords);
    glm_translate(view_mat, player_world_coords);
  }
  glm_scale_uni(model_mat, FBO_QUAD_WIDTH * T_WIDTH / 2.0);

  render_fbo_entity(player_ship, fbo_model_mat, model_mat, fbo_view_mat,
                    view_mat, persp_proj, ortho_proj);
  render_ripple(player_chunks[PLAYER_CHUNK], e_player.ship_coords,
                e_player.ship_direction, e_player.ship_moving);
}

void render_enemy_ship(E_ENEMY *es, unsigned int chunk) {
  render_e_npc(enemy_ship, es->chunk, es->coords, es->direction, 0.70);
  render_ripple(chunk, es->coords, es->direction, es->moving);
}

void render_trade_ship(TRADE_SHIP *ts, unsigned int chunk) {
  float scale = 0.5;
  if (ts->death_animation >= 0.0) {
    scale = scale * ts->death_animation;
  }
  render_e_npc(trade_ship, ts->chunk_coords, ts->coords, ts->direction, scale);
  render_ripple(chunk, ts->coords, ts->direction, ts->moving);
}

void render_player() {
  if ((mode == EXPLORATION && !e_player.embarked) || mode == COMBAT) {
    mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 player_rot = GLM_MAT4_IDENTITY_INIT;
    vec3 player_dir = GLM_VEC2_ZERO_INIT;
    if (mode == EXPLORATION) {
      player_dir[0] = e_player.direction[0];
      player_dir[1] = e_player.direction[1];
    } else {
      player_dir[0] = c_player.direction[0];
      player_dir[1] = c_player.direction[1];
    }
    calc_rot_mat(player_dir, player_rot);
    glm_scale_uni(fbo_model_mat, 0.25);
    glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
    glm_mat4_mul(fbo_model_mat, player_rot, fbo_model_mat);

    mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(fbo_view_mat, -3.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    vec3 depth_offset = { 0.0, 0.0, AVATAR_DEPTH };
    glm_translate(model_mat, depth_offset);
    glm_scale_uni(model_mat, FBO_QUAD_WIDTH * T_WIDTH / 2.0);
    glm_translate_y(model_mat, 0.1);

    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

    MODEL *player_model = player[STILL];
    if (e_player.moving) {
      unsigned int frame = ((unsigned int) (glfwGetTime() * 10)) % 2;
      player_model = player[WALK_1 + frame];
    }

    render_fbo_entity(player_model, fbo_model_mat, model_mat, fbo_view_mat,
                      view_mat, persp_proj, ortho_proj);
  }
}

void render_unit(C_UNIT *unit) {
  float scale = 0.25;
  if (unit->death_animation >= 0.0) {
    scale = scale * unit->death_animation;
  }
  if (unit->type == ENEMY) {
    MODEL *enemy_model = enemy[STILL];
    if (unit->moving) {
      unsigned int frame = ((unsigned int) (glfwGetTime() * 10)) % 2;
      enemy_model = enemy[WALK_1 + frame];
    }

    render_c_npc(enemy_model, unit->coords, unit->direction, scale);
  } else if (unit->type == ALLY) {
    MODEL *merc_model = mercenary[STILL];
    if (unit->moving) {
      unsigned int frame = ((unsigned int) (glfwGetTime() * 10)) % 2;
      merc_model = mercenary[WALK_1 + frame];
    }

    render_c_npc(merc_model, unit->coords, unit->direction, scale);
  }
}

void render_loot(L_UNIT *loot) {
  render_c_npc(chest, loot->coords, (vec2) { 0.0, 1.0 }, 0.25);
}

void render_merchant(MERCHANT *m) {
  vec2 merchant_dir = { 0.0, -1.0 };
  render_e_npc(merchant, m->chunk, m->coords, merchant_dir, 0.25);
}

void render_e_npc(MODEL *model, ivec2 chunk, vec2 coords, vec2 direction,
                  float scale) {
  mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 npc_rot = GLM_MAT4_IDENTITY_INIT;
  vec3 npc_dir = { direction[0], direction[1], 0.0f };
  calc_rot_mat(npc_dir, npc_rot);
  glm_scale_uni(fbo_model_mat, scale);
  glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
  glm_mat4_mul(fbo_model_mat, npc_rot, fbo_model_mat);

  mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate_z(fbo_view_mat, -3.0);

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 world_coords = { 0.0, 0.0, AVATAR_DEPTH };
  chunk_to_world(chunk, coords, world_coords);
  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, FBO_QUAD_WIDTH * T_WIDTH / 2.0);
  //glm_translate_y(model_mat, 0.1);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);

  render_fbo_entity(model, fbo_model_mat, model_mat, fbo_view_mat,
                    view_mat, persp_proj, ortho_proj);
}

void render_c_npc(MODEL *model, vec2 coords, vec2 direction, float scale) {
  mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 npc_rot = GLM_MAT4_IDENTITY_INIT;
  vec3 npc_dir = { direction[0], direction[1], 0.0f };
  calc_rot_mat(npc_dir, npc_rot);
  glm_scale_uni(fbo_model_mat, scale);
  glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
  glm_mat4_mul(fbo_model_mat, npc_rot, fbo_model_mat);

  mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate_z(fbo_view_mat, -3.0);

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 npc_coords = { 0.0, 0.0, AVATAR_DEPTH };
  glm_vec2_scale(coords, T_WIDTH, npc_coords);
  glm_translate(model_mat, npc_coords);
  glm_scale_uni(model_mat, FBO_QUAD_WIDTH * T_WIDTH / 2.0);
  glm_translate_y(model_mat, 0.1);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_coords = GLM_VEC2_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  glm_vec3_negate(player_coords);
  glm_translate(view_mat, player_coords);

  render_fbo_entity(model, fbo_model_mat, model_mat, fbo_view_mat,
                    view_mat, persp_proj, ortho_proj);
}

void render_resource(vec2 position, ISLAND *island, REC_IDS r_type) {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 world_coords = {0.0, 0.0, OBSTACLE_DEPTH };
  vec2 island_tile = {
    island->coords[0] + position[0],
    island->coords[1] + position[1]
  };
  chunk_to_world(island->chunk, island_tile, world_coords);
  world_coords[0] = world_coords[0] + (0.5 * 2 * T_WIDTH);
  world_coords[1] = world_coords[1] - (0.5 * 2 * T_WIDTH);
  // world_coords[0] = 25.0 + (0.5 * 2 * T_WIDTH);
  // world_coords[1] = 74.0 - (0.5 * 2 * T_WIDTH);

  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, 0.5 * 2 * T_WIDTH);

  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);

  quad->texture = resource_textures[resource_to_buffer(r_type)];

  glUseProgram(pixel_shader);
  set_float("num_pixels", 256.0, pixel_shader);
  set_mat4("model", model_mat, pixel_shader);
  set_mat4("view", view_mat, pixel_shader);
  set_mat4("proj", ortho_proj, pixel_shader);
  draw_model(quad, pixel_shader);
}

void render_hunger_bar() {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  float ratio_x = RES_X / BASE_RES_X;
  float ratio_y = RES_Y / BASE_RES_Y;
  vec3 hunger_bar_pos = GLM_VEC3_ZERO_INIT;
  hunger_bar_pos[0] = -ratio_x + 0.2;
  hunger_bar_pos[2] = UI_DEPTH;
  if (!console_input_enabled) {
    hunger_bar_pos[1] = -ratio_y * 0.9;
  } else {
    hunger_bar_pos[1] = -ratio_y * 0.675;
  }

  glm_translate(model_mat, hunger_bar_pos);
  vec3 scale = {0.25, 0.25, 1.0};
  glm_scale(model_mat, scale);

  int tex_num = (int) (e_player.hunger / 11.0) + 1;
  if (e_player.hunger > 100.0) {
    tex_num = 10;
  } else if (e_player.hunger <= 0.0) {
    tex_num = 0;
  }
  quad->texture = hunger_bar_textures[tex_num];

  glUseProgram(std_shader);
  set_mat4("model", model_mat, std_shader);
  set_mat4("view", view_mat, std_shader);
  set_mat4("proj", ortho_proj, std_shader);
  draw_model(quad, std_shader);
}

void render_player_health_bar() {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  float ratio_x = RES_X / BASE_RES_X;
  float ratio_y = RES_Y / BASE_RES_Y;
  vec3 health_bar_pos = GLM_VEC3_ZERO_INIT;
  health_bar_pos[0] = -ratio_x + 0.6;
  health_bar_pos[2] = UI_DEPTH;
  if (!console_input_enabled) {
    health_bar_pos[1] = -ratio_y * 0.9;
  } else {
    health_bar_pos[1] = -ratio_y * 0.675;
  }

  glm_translate(model_mat, health_bar_pos);
  vec3 scale = {0.25, 0.25, 1.0};
  glm_scale(model_mat, scale);

  float actual_health = glm_clamp(c_player.health, 0.0, c_player.max_health);

  int tex_num = (int) (actual_health / (c_player.max_health /(100.0/11.0))) + 1;
  if (actual_health <= 0.0) {
    tex_num = 0;
  }
  quad->texture = health_bar_textures[tex_num];

  glUseProgram(std_shader);
  set_mat4("model", model_mat, std_shader);
  set_mat4("view", view_mat, std_shader);
  set_mat4("proj", ortho_proj, std_shader);
  draw_model(quad, std_shader);
}

void render_player_health_bar_combat() {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  float ratio_x = RES_X / BASE_RES_X;
  float ratio_y = RES_Y / BASE_RES_Y;
  vec3 health_bar_pos = GLM_VEC3_ZERO_INIT;
  health_bar_pos[0] = -ratio_x + 0.2;
  if (!console_input_enabled) {
    health_bar_pos[1] = -ratio_y * 0.9;
  } else {
    health_bar_pos[1] = -ratio_y * 0.675;
  }
  health_bar_pos[2] = UI_DEPTH;

  glm_translate(model_mat, health_bar_pos);
  vec3 scale = {0.25, 0.25, 1.0};
  glm_scale(model_mat, scale);

  float actual_health = glm_clamp(c_player.health, 0.0, 100.0);

  int tex_num = (int) (actual_health / 11.0) + 1;
  if (actual_health <= 0.0) {
    tex_num = 0;
  }
  quad->texture = health_bar_textures[tex_num];

  glUseProgram(std_shader);
  set_mat4("model", model_mat, std_shader);
  set_mat4("view", view_mat, std_shader);
  set_mat4("proj", ortho_proj, std_shader);
  draw_model(quad, std_shader);
}

void render_health_bar_background(vec2 position) {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  vec3 health_bar_pos = { 0.0, 0.0, AVATAR_DEPTH };
  glm_vec2_copy(position, health_bar_pos);
  glm_translate(model_mat, health_bar_pos);
  vec3 scale = {T_WIDTH, 0.15*T_WIDTH, 1.0};
  glm_scale(model_mat, scale);

  vec3 player_coords = GLM_VEC3_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  glm_vec2_negate(player_coords);
  glm_translate(view_mat, player_coords);

  vec3 health_bar_col = { 0.13, 0.13, 0.13 };
  glUseProgram(color_shader);
  set_mat4("model", model_mat, color_shader);
  set_mat4("view", view_mat, color_shader);
  set_mat4("proj", ortho_proj, color_shader);
  set_vec3("color", health_bar_col, color_shader);
  draw_model(quad, color_shader);
}

void render_health_bar_filled(vec2 position, float max_life, float life) {
  float one_scale = T_WIDTH/max_life;
  float hp_bar_val = one_scale * life;
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  vec3 health_bar_pos = { 0.0, 0.0, AVATAR_DEPTH };
  glm_vec2_copy(position, health_bar_pos);
  glm_translate(model_mat, (vec3) {
                  health_bar_pos[0]-((T_WIDTH-hp_bar_val)),
                  health_bar_pos[1],
                  health_bar_pos[2]
                });
  vec3 scale = {hp_bar_val, 0.15*T_WIDTH, 1.0};
  glm_scale(model_mat, scale);

  vec3 player_coords = GLM_VEC3_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  glm_vec2_negate(player_coords);
  glm_translate(view_mat, player_coords);

  vec3 health_bar_col = { 1.0, 0.0, 0.0 };
  glUseProgram(color_shader);
  set_mat4("model", model_mat, color_shader);
  set_mat4("view", view_mat, color_shader);
  set_mat4("proj", ortho_proj, color_shader);
  set_vec3("color", health_bar_col, color_shader);
  draw_model(quad, color_shader);
}

void render_obstacle(MODEL *model, ivec2 chunk, vec2 coords, float scale) {
  mat4 fbo_model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_scale_uni(fbo_model_mat, scale);
  glm_rotate_x(fbo_model_mat, glm_rad(-50.0), fbo_model_mat);
  //glm_rotate_z(fbo_model_mat, glm_rad(180.0), fbo_model_mat);

  mat4 fbo_view_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate_z(fbo_view_mat, -3.0);

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 world_coords = { 0.0, 0.0, OBSTACLE_DEPTH };
  chunk_to_world(chunk, coords, world_coords);
  world_coords[0] += 0.5 * T_WIDTH;
  world_coords[1] -= 0.5 * T_WIDTH;
  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, FBO_QUAD_WIDTH * T_WIDTH / 2.0);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);

  render_fbo_entity(model, fbo_model_mat, model_mat, fbo_view_mat,
                    view_mat, persp_proj, ortho_proj);
}

void render_ui(UI_COMPONENT *comp) {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  // Dimensions of UI component (width, height)
  vec3 comp_scale = { 0.5 * screen_scale[0] * comp->width,
                      0.5 * screen_scale[1] * comp->height, 1.0 };
  // Location of "pivot" point of ui component
  vec3 comp_pivot = { 0.0, 0.0, UI_DEPTH };
  // Number of characters in ui component text
  int text_len = 0;
  // Scale text based on screen size
  float screen_text_scale = get_screen_text_scale();
  // Width of ui component text
  float text_width = 0.0;
  float text_height = 0.0;
  if (comp->text) {
    text_len = strlen(comp->text);
    text_width = get_text_width(comp->text, text_len) * comp->text_scale *
                 screen_text_scale;
    // All characters have uniform height, so just scale by height of first
    // character in font
    text_height = font[0].height * comp->text_scale * screen_text_scale;
  }

  // Dynamic scaling of width and height
  if (comp_scale[0] == 0.0) {
    comp_scale[0] = (text_width * 0.5) + comp->text_padding;
  }
  if (comp_scale[1] == 0.0) {
    comp_scale[1] = (text_height * 0.5) + comp->text_padding;
  }

  vec2 screen_space_pos = {
    comp->position[0] * screen_scale[0],
    comp->position[1] * screen_scale[1]
  };

  vec2 screen_space_offset = {
    UI_PIVOT_OFFSETS[comp->pivot][0],
    UI_PIVOT_OFFSETS[comp->pivot][1]
  };

  // Adjust comp position based on pivot point
  vec3 comp_offset = { screen_space_offset[0] * comp_scale[0],
                       screen_space_offset[1] * comp_scale[1], 0.0 };
  glm_vec2_add(screen_space_pos, comp_offset, comp_pivot);

  glm_translate(model_mat, comp_pivot);
  glm_scale(model_mat, comp_scale);

  if (comp->text) {
    // Location of "pivot" point of text, relative to center of UI component
    vec3 text_pivot = { 0.0, 0.0, TEXT_DEPTH };
    if (comp->text_anchor == T_CENTER) {
      text_pivot[0] = -0.5 * text_width;
    } else if (comp->text_anchor == T_LEFT) {
      text_pivot[0] = -comp_scale[0] + comp->text_padding;
    } else if (comp->text_anchor == T_RIGHT) {
      text_pivot[0] = comp_scale[0] - (text_width + comp->text_padding);
    }
    glm_vec2_add(text_pivot, comp_pivot, text_pivot);
    mat4 text_model_mat = GLM_MAT4_IDENTITY_INIT;
    glm_translate(text_model_mat, text_pivot);
    glm_scale_uni(text_model_mat, comp->text_scale * screen_text_scale);

    render_text(comp->text, text_len, text_model_mat);
  }

  if (comp->textured) {
    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

    glUseProgram(std_shader);
    set_mat4("model", model_mat, std_shader);
    set_mat4("view", view_mat, std_shader);
    set_mat4("proj", ortho_proj, std_shader);
    quad->texture = comp->texture;
    draw_model(quad, std_shader);
  }
}

void render_text(char *text, int text_len, mat4 text_model) {
  mat4 char_model = GLM_MAT4_IDENTITY_INIT;
  glm_rotate_z(char_model, glm_rad(90.0), char_model);
  glm_rotate_x(char_model, glm_rad(90.0), char_model);

  // Render each character of the text
  for (unsigned int i = 0; i < text_len; i++) {
    int char_index = text[i] - ' ';
    if (text[i] < ' ' || char_index >= FONT_LEN) {
      char_index = ' ';
    }

    glm_translate_z(char_model, font[char_index].width / 2.0);

    mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
    mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
    glm_mat4_mul(text_model, char_model, model_mat);

    glUseProgram(text_shader);
    set_mat4("model", model_mat, text_shader);
    set_mat4("view", view_mat, text_shader);
    set_mat4("proj", ortho_proj, text_shader);
    draw_model(font[char_index].model, text_shader);

    glm_translate_z(char_model, font[char_index].width / 2.0);
    if (i < text_len - 1) {
      glm_translate_z(char_model, 1.0 / 160.0);
    }
  }
}

void render_fbo_entity(
    MODEL *entity,
    mat4 fbo_model, mat4 model,
    mat4 fbo_view, mat4 view,
    mat4 fbo_proj, mat4 proj
) {
  glBindFramebuffer(GL_FRAMEBUFFER, entity_framebuffer.FBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(std_shader);
  set_mat4("model", fbo_model, std_shader);
  set_mat4("view", fbo_view, std_shader);
  set_mat4("proj", fbo_proj, std_shader);
  draw_model(entity, std_shader);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(pixel_shader);
  set_float("num_pixels", 1024.0, pixel_shader);
  set_mat4("model", model, pixel_shader);
  set_mat4("view", view, pixel_shader);
  set_mat4("proj", proj, pixel_shader);
  quad->texture = entity_framebuffer.color_texture;
  draw_model(quad, pixel_shader);
}

void render_ripple(unsigned int chunk_index, vec2 chunk_coords, vec2 direction,
                   int moving) {
  unsigned int num_tiles = RIPPLE_WIDTH * RIPPLE_WIDTH;
  int tiles[num_tiles];
  ivec2 top_left = {
    chunk_coords[0] - (RIPPLE_WIDTH / 2),
    chunk_coords[1] - (RIPPLE_WIDTH / 2)
  };
  vec2 cur_tile = GLM_VEC2_ZERO_INIT;
  for (int i = 0; i < num_tiles; i++) {
    cur_tile[0] = top_left[0] + (i % RIPPLE_WIDTH);
    cur_tile[1] = top_left[1] + (i / RIPPLE_WIDTH);
    tiles[i] = get_tile(chunk_index, cur_tile);
  }

  vec3 world_coords = {
    top_left[0] + (RIPPLE_WIDTH / 2),
    top_left[1] + (RIPPLE_WIDTH / 2),
    EFFECT_DEPTH
  };
  chunk_to_world(chunk_buffer[chunk_index].coords, world_coords, world_coords);
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, 0.5 * T_WIDTH * RIPPLE_WIDTH);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);

  vec2 ship_pos = {
    (chunk_coords[0] - top_left[0]) / RIPPLE_WIDTH,
    (chunk_coords[1] - top_left[1]) / RIPPLE_WIDTH
  };

  // Calculate matrix to rotate vectors to coordinate space where the ship's
  // direction is the y vector
  vec3 ship_z = { 0.0, 0.0, 1.0 };
  vec3 ship_y = GLM_VEC3_ZERO_INIT;
  vec3 ship_x = GLM_VEC3_ZERO_INIT;
  glm_vec2_normalize_to(direction, ship_y);
  // In tile chunk coordinates, the y direction is downward, so we must negate
  // the ship direction in the y
  ship_y[1] *= -1.0;
  glm_vec3_cross(ship_y, ship_z, ship_x);
  mat4 ship_rot = GLM_MAT4_IDENTITY_INIT;
  glm_vec3_copy(ship_x, ship_rot[0]);
  glm_vec3_copy(ship_y, ship_rot[1]);
  glm_vec3_copy(ship_z, ship_rot[2]);

  /*
  printf("dir: (%.2f, %.2f)\n", e_player.ship_direction[0],
         e_player.ship_direction[1]);
  for (int i = 0; i < 4; i++) {
    printf("|%.1f, %.1f, %.1f, %.1f|\n",
           ship_rot[i][0],
           ship_rot[i][1],
           ship_rot[i][2],
           ship_rot[i][3]);
  }
  printf("\n");
  fflush(stdout);
  */

  glUseProgram(ripple_shader);
  set_mat4("model", model_mat, ripple_shader);
  set_mat4("view", view_mat, ripple_shader);
  set_mat4("proj", ortho_proj, ripple_shader);
  set_mat4("ship_rot", ship_rot, ripple_shader);
  set_vec2("ship_pos", ship_pos, ripple_shader);
  set_iarr("tiles", tiles, num_tiles, ripple_shader);
  set_float("time", glfwGetTime(), ripple_shader);
  set_int("moving", moving, ripple_shader);
  draw_model(quad, ripple_shader);
}

void render_weather() {
  vec3 weather_coords = { 0.0, 0.0, WEATHER_DEPTH };
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model_mat, weather_coords);
  glm_scale(model_mat, (vec3) { RES_X / BASE_RES_X, RES_Y / BASE_RES_Y, 1.0 });

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  glUseProgram(weather_shader);
  set_vec2("player_pos", player_world_coords, weather_shader);
  set_float("time", glfwGetTime(), weather_shader);
  set_float("T_WIDTH", T_WIDTH, weather_shader);
  set_float("ratio_x", RES_X / BASE_RES_X, weather_shader);
  set_float("ratio_y", RES_Y / BASE_RES_Y, weather_shader);
  set_float("res_x", RES_X, weather_shader);
  set_float("res_y", RES_Y, weather_shader);
  set_mat4("model", model_mat, weather_shader);
  set_mat4("view", view_mat, weather_shader);
  set_mat4("proj", ortho_proj, weather_shader);
  draw_model(quad, weather_shader);
}

void render_chunk(ivec2 chunk) {
  vec3 world_coords = { 0.0, 0.0, WORLD_DEPTH};
  vec2 tile_coords = GLM_VEC2_ZERO_INIT;
  chunk_to_world(chunk, tile_coords, world_coords);
  world_coords[0] = world_coords[0] + (T_WIDTH * C_WIDTH * 0.5);
  world_coords[1] = world_coords[1] - (T_WIDTH * C_WIDTH * 0.5);

  /*
  float c_val = 0.0f;
  if (chunk[0] % 2 == 0) {
    if (chunk[1] % 2 == 0) {
      c_val = 1.0;
    } else {
      c_val = 0.5;
    }
  } else {
    if (chunk[1] % 2 == 0) {
      c_val = 0.5;
    } else {
      c_val = 1.0;
    }
  }
  */

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, 0.5 * T_WIDTH * C_WIDTH);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);
  glm_translate_z(view_mat, -1.0f);

  float time = glfwGetTime();
  float puddle_offset = floor(time / 0.5);

  float threshold = (H*sin((2*PI/WAVE_PERIOD) * time))+K;
  if (((threshold >= K+H && threshold <= K+H+0.0001) ||
      (threshold <= K+H && threshold >= K+H-0.0001)) && !incremented_1) {
    incremented_1 = 1;
    wave_offset_1 += C;
    if (wave_offset_1 >= 20.0) {
      wave_offset_1 = 0.0;
    }
  } else if (!(threshold >= K+H && threshold <= K+H+0.0001) &&
             !(threshold <= K+H && threshold >= K+H-0.0001)) {
    incremented_1 = 0;
  }
  if (((threshold >= K-H && threshold <= K-H+0.0001) ||
      (threshold <= K-H && threshold >= K-H-0.0001)) && !incremented_2) {
    incremented_2 = 1;
    wave_offset_2 += C;
    if (wave_offset_1 >= 20.0) {
      wave_offset_1 = 0.0;
    }
  } else if (!(threshold >= K-H && threshold <= K-H+0.0001) &&
             !(threshold <= K-H && threshold >= K-H-0.0001)) {
    incremented_2 = 0;
  }

  unsigned int chunk_shader = 0;
  if (weather == CLEAR) {
    chunk_shader = chunk_wave_shader;
    glUseProgram(chunk_shader);
    set_vec2("chunk_coords", world_coords, chunk_shader);
    set_float("threshold", threshold, chunk_shader);
    set_float("offset_1", wave_offset_1, chunk_shader);
    set_float("offset_2", wave_offset_2, chunk_shader);
  } else {
    chunk_shader = chunk_rain_shader;
    glUseProgram(chunk_shader);
    set_float("puddle_offset", puddle_offset, chunk_shader);
  }
  set_mat4("model", model_mat, chunk_shader);
  set_mat4("view", view_mat, chunk_shader);
  set_mat4("proj", ortho_proj, chunk_shader);
  set_float("time", time, chunk_shader);
  //set_float("chunk", c_val, chunk_shader);
  draw_model(quad, chunk_shader);
}

void render_island(ISLAND *island) {
  vec3 world_coords = {0.0, 0.0, WORLD_DEPTH};
  vec2 island_tile = { island->coords[0], island->coords[1] };
  chunk_to_world(island->chunk, island_tile, world_coords);
  world_coords[0] = world_coords[0] + (0.5 * T_WIDTH * I_WIDTH);
  world_coords[1] = world_coords[1] - (0.5 * T_WIDTH * I_WIDTH);

  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model_mat, world_coords);
  glm_scale_uni(model_mat, 0.5 * T_WIDTH * I_WIDTH);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 player_world_coords = GLM_VEC2_ZERO_INIT;
  if (e_player.embarked) {
    chunk_to_world(e_player.ship_chunk, e_player.ship_coords,
                   player_world_coords);
  } else {
    chunk_to_world(e_player.chunk, e_player.coords, player_world_coords);
  }
  glm_vec3_negate(player_world_coords);
  glm_translate(view_mat, player_world_coords);

  glUseProgram(std_shader);
  set_mat4("model", model_mat, std_shader);
  set_mat4("view", view_mat, std_shader);
  set_mat4("proj", ortho_proj, std_shader);
  quad->texture = island->texture;
  draw_model(quad, std_shader);

  if (island->chunk[0] == 0 && island->chunk[1] == 0) {
    glm_mat4_identity(model_mat);
    vec3 house_coords = { 0.0, 0.0, OBSTACLE_DEPTH };
    chunk_to_world(island->chunk, house_tile, house_coords);
    glm_translate(model_mat, house_coords);
    glm_translate_x(model_mat, 2.0 * T_WIDTH);
    glm_translate_y(model_mat, 4.0 * T_WIDTH);
    glm_scale_uni(model_mat, T_WIDTH * 5.0);

    glUseProgram(pixel_shader);
    set_mat4("model", model_mat, pixel_shader);
    set_mat4("view", view_mat, pixel_shader);
    set_mat4("proj", ortho_proj, pixel_shader);
    draw_model(house, pixel_shader);

    render_obstacle(chest, island->chunk, home_box_tile, 0.25);
  }
}

void render_arena() {
  // Render ocean
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  vec3 scale = {
    screen_scale[0], screen_scale[1], 1.0
  };
  glm_translate_z(model_mat, WORLD_DEPTH);
  glm_scale(model_mat, scale);

  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  vec3 ocean_col = { 3.0, 157.0, 252.0 };
  glm_vec3_normalize(ocean_col);

  glUseProgram(color_shader);
  set_mat4("model", model_mat, color_shader);
  set_mat4("view", view_mat, color_shader);
  set_mat4("proj", ortho_proj, color_shader);
  set_vec3("color", ocean_col, color_shader);
  draw_model(quad, color_shader);

  // Render arena floor
  glm_mat4_identity(model_mat);
  scale[0] = 0.5 * T_WIDTH * arena_dimensions[0];
  scale[1] = 0.5 * T_WIDTH * arena_dimensions[1];
  vec3 floor_pos = { 0.0, 0.0, OBSTACLE_DEPTH };
  glm_translate(model_mat, floor_pos);
  glm_scale(model_mat, scale);

  vec3 player_coords = GLM_VEC3_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  glm_vec2_negate(player_coords);
  glm_translate(view_mat, player_coords);

  vec3 floor_col = { 120.0, 94.0, 23.0 };
  glm_vec3_normalize(floor_col);

  set_mat4("model", model_mat, color_shader);
  set_mat4("view", view_mat, color_shader);
  set_mat4("proj", ortho_proj, color_shader);
  set_vec3("color", floor_col, color_shader);
  draw_model(quad, color_shader);

  // Render arena wall
  glm_mat4_identity(model_mat);
  scale[1] *= 0.25;
  glm_translate_y(model_mat, 0.625 * T_WIDTH * arena_dimensions[1]);
  glm_translate_z(model_mat, OBSTACLE_DEPTH);
  glm_scale(model_mat, scale);

  vec3 wall_col = { 235.0, 206.0, 129.0 };
  glm_vec3_normalize(wall_col);

  set_mat4("model", model_mat, color_shader);
  set_vec3("color", wall_col, color_shader);
  draw_model(quad, color_shader);
}

void render_hitbox(vec2 world_coords, float radius) {
  mat4 model_mat = GLM_MAT4_IDENTITY_INIT;
  mat4 view_mat = GLM_MAT4_IDENTITY_INIT;

  vec3 hitbox_pos = { 0.0, 0.0, AVATAR_DEPTH };
  glm_vec2_copy(world_coords, hitbox_pos);
  glm_translate(model_mat, hitbox_pos);
  glm_scale_uni(model_mat, T_WIDTH * radius);

  vec3 player_coords = GLM_VEC3_ZERO_INIT;
  glm_vec2_scale(c_player.coords, T_WIDTH, player_coords);
  glm_vec2_negate(player_coords);
  glm_translate(view_mat, player_coords);

  glUseProgram(pixel_shader);
  quad->texture = bullet_texture;
  set_mat4("model", model_mat, pixel_shader);
  set_mat4("view", view_mat, pixel_shader);
  set_mat4("proj", ortho_proj, pixel_shader);
  draw_model(quad, pixel_shader);
}

/*
  Helper function for compiling a shader program
*/
unsigned int shader_init(const char *vs, const char *fs) {
  int vs_len = strlen(vs);
  unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vs, &vs_len);
  glCompileShader(vert_shader);

  int status = 0;
  char info_log[1024];
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(vert_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Vertex shader error for %s:\n%s\n", vs, info_log);

    return INVALID_SHADER;
  }

  int fs_len = strlen(fs);
  unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &fs, &fs_len);
  glCompileShader(frag_shader);

  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(frag_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Fragment shader error for %s:\n%s\n", fs, info_log);

    return INVALID_SHADER;
  }

  unsigned int shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vert_shader);
  glAttachShader(shader_prog, frag_shader);
  glLinkProgram(shader_prog);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  return shader_prog;
}

/*
  Helper function to set up framebuffer and respective color, depth and stencil
  textures
*/
FRAMEBUFFER framebuffer_init() {
  FRAMEBUFFER fb;

  glGenFramebuffers(1, &fb.FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, fb.FBO);

  glGenTextures(1, &fb.color_texture);
  glBindTexture(GL_TEXTURE_2D, fb.color_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, RES_X, RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &fb.depth_stencil_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, fb.depth_stencil_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RES_X, RES_Y);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         fb.color_texture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, fb.depth_stencil_rbo);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return fb;
}

void refresh_framebuffers() {
  glDeleteTextures(1, &entity_framebuffer.color_texture);
  glDeleteRenderbuffers(1, &entity_framebuffer.depth_stencil_rbo);
  glDeleteFramebuffers(1, &entity_framebuffer.FBO);
  entity_framebuffer = framebuffer_init();
}

void calc_rot_mat(vec3 dir, mat4 dest) {
  vec3 z_cross_dir = GLM_VEC3_ZERO_INIT;
  glm_vec3_cross(Z, dir, z_cross_dir);
  mat3 rot = GLM_MAT3_IDENTITY_INIT;
  glm_vec3_copy(z_cross_dir, rot[0]);
  glm_vec3_copy(dir, rot[1]);
  glm_vec3_copy(Z, rot[2]);
  glm_mat4_identity(dest);
  glm_mat4_ins3(rot, dest);
}

void calc_screen_scale() {
  vec3 screen_res = {
    ((float) RES_X) / BASE_RES_X,
    ((float) RES_Y) / BASE_RES_Y,
    1.0
  };
  glm_ortho(-screen_res[0], screen_res[0], -screen_res[1], screen_res[1], 0.0,
            100.0, ortho_proj);

  glm_vec2_copy(screen_res, screen_scale);
}

float get_text_width(char *text, int text_len) {
  // Calculate total width of text to determine translation needed to center
  // text
  float total_width = 0.0;
  for (unsigned int i = 0; i < text_len; i++) {
    int char_index = text[i] - ' ';
    if (text[i] < ' ') {
      char_index = ' ';
    }
    total_width += font[char_index].width;
  }
  // Account for space in between letters
  total_width += ((text_len - 1) * (1.0 / 160.0));
  return total_width;
}

void get_ui_min_max(UI_COMPONENT *comp, vec4 dest) {
  vec2 comp_scale = { 0.5 * comp->width, 0.5 * comp->height };
  vec2 comp_pivot = { 0.0, 0.0 };
  int text_len = 0;
  float text_width = 0.0;
  float text_height = 0.0;
  if (comp->text) {
    text_len = strlen(comp->text);
    text_width = get_text_width(comp->text, text_len) * comp->text_scale;
    text_height = font[0].height * comp->text_scale;
  }

  // Dynamic scaling of width and height
  if (comp_scale[0] == 0.0) {
    comp_scale[0] = (text_width * 0.5) + comp->text_padding;
  }
  if (comp_scale[1] == 0.0) {
    comp_scale[1] = (text_height * 0.5) + comp->text_padding;
  }

  // Adjust comp position based on pivot point
  vec2 comp_offset = { UI_PIVOT_OFFSETS[comp->pivot][0] * comp_scale[0],
                       UI_PIVOT_OFFSETS[comp->pivot][1] * comp_scale[1] };
  glm_vec2_add(comp->position, comp_offset, comp_pivot);

  dest[X_MIN] = comp_pivot[0] - comp_scale[0];
  dest[X_MAX] = comp_pivot[0] + comp_scale[0];
  dest[Y_MIN] = comp_pivot[1] - comp_scale[1];
  dest[Y_MAX] = comp_pivot[1] + comp_scale[1];
}

float get_screen_text_scale() {
  float screen_text_scale = RES_X / BASE_RES_X;
  if (screen_text_scale < MIN_TEXT_SCALE) {
    screen_text_scale = MIN_TEXT_SCALE;
  }
  return screen_text_scale;
}

/*
  The following functions are helpers to consicely set up uniform variables in
  shaders
*/
void set_mat4(char *name, mat4 matrix, unsigned int shader) {
  glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                     (float *) matrix);
}

void set_mat3(char *name, mat3 matrix, unsigned int shader) {
  glUniformMatrix3fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                     (float *) matrix);
}

void set_vec4(char *name, vec4 matrix, unsigned int shader) {
  glUniform4fv(glGetUniformLocation(shader, name), 1, (float *) matrix);
}

void set_vec3(char *name, vec3 v, unsigned int shader) {
  glUniform3fv(glGetUniformLocation(shader, name), 1, (float *) v);
}

void set_vec2(char *name, vec2 v, unsigned int shader) {
  glUniform2fv(glGetUniformLocation(shader, name), 1, (float *) v);
}

void set_float(char *name, float val, unsigned int shader) {
  glUniform1f(glGetUniformLocation(shader, name), val);
}

void set_int(char *name, int val, unsigned int shader) {
  glUniform1i(glGetUniformLocation(shader, name), val);
}

void set_uint(char *name, unsigned int val, unsigned int shader) {
  glUniform1ui(glGetUniformLocation(shader, name), val);
}

void set_iarr(char *name, int *arr, unsigned int count, unsigned int shader) {
  glUniform1iv(glGetUniformLocation(shader, name), count, arr);
}

void set_texture(char *name, unsigned int tex, unsigned int shader, int unit) {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, tex);
  glUniform1i(glGetUniformLocation(shader, name), unit);
}
