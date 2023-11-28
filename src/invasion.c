#include <invasion.h>

void init_invasion_ui() {
  ransom_buffer[0] = '\0';

  vec2 ransom_msg_pos = { 0.55, 0.01 };
  init_menu(
    ransom_msg_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    ransom_buffer, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.35, // width
    0.25, // height
    PIVOT_BOTTOM_LEFT, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(RANSOM_MSG_POS) // dest
  );

  vec2 ransom_btn_pos = { 0.55, -0.01 };
  init_menu(
    ransom_btn_pos, // position
    pay_ransom, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Pay Ransom", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.35, // width
    0.15, // height
    PIVOT_TOP_LEFT, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(RANSOM_BUTTON_POS) // dest
  );
}

void open_ransom_menu() {
  float ransom_price = calc_ransom_price();
  if (ransom_price) {
    get_ui_component_by_ID(RANSOM_MSG_POS)->enabled = 1;
    get_ui_component_by_ID(RANSOM_BUTTON_POS)->enabled = 1;
    snprintf(ransom_buffer, RANSOM_BUFF_SIZE, "Ransom Price: %.1f",
             ransom_price);
  } else {
    get_ui_component_by_ID(RANSOM_MSG_POS)->enabled = 0;
    get_ui_component_by_ID(RANSOM_BUTTON_POS)->enabled = 0;
  }
}

void refresh_ransom_menu() {
  float ransom_price = calc_ransom_price();
  snprintf(ransom_buffer, RANSOM_BUFF_SIZE, "Ransom Price: %.1f",
           ransom_price);
}

void close_ransom_menu() {
  get_ui_component_by_ID(RANSOM_MSG_POS)->enabled = 0;
  get_ui_component_by_ID(RANSOM_BUTTON_POS)->enabled = 0;
  ransom_buffer[0] = '\0';
}

void pay_ransom() {
  float ransom_price = calc_ransom_price();
  float player_money = get_player_money();

  if (player_money < ransom_price) {
    set_prompt("Not enough money!");
    return;
  }

  player_money -= ransom_price;
  remove_money(e_player.inventory, MAX_PLAYER_INV_SIZE);
  give_player_copper(player_money);
  coalesce_currency(e_player.inventory, MAX_PLAYER_INV_SIZE);

  CHUNK *home_chunk = chunk_buffer + home_chunk_index;
  ISLAND *home_island = home_chunk->islands + HOME_ISLAND_INDEX;
  vec2 island_coords = { home_island->coords[0],
                         home_island->coords[1] };
  vec2 island_coords_world = GLM_VEC2_ZERO_INIT;
  chunk_to_world(home_chunk->coords, island_coords, island_coords_world);

  for (unsigned int i = 0; i < home_chunk->num_enemies; i++) {
    E_ENEMY *cur_enemy = home_chunk->enemies + i;
    vec2 world_coords = GLM_VEC2_ZERO_INIT;
    chunk_to_world(home_chunk->coords, cur_enemy->coords, world_coords);
    if (world_coords[0] <= island_coords_world[0] + (I_WIDTH * T_WIDTH) &&
        world_coords[0] >= island_coords_world[0] &&
        world_coords[1] <= island_coords_world[1] &&
        world_coords[1] >= island_coords_world[1] - (I_WIDTH * T_WIDTH)) {
      home_chunk->num_enemies--;
      home_chunk->enemies[i] = home_chunk->enemies[home_chunk->num_enemies];
      i--;
    }
  }
  close_ransom_menu();
}
