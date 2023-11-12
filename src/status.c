#include <status.h>

/*
                                   STATUS
Implements the functionality for shows player status.
*/

/*
Init status bar
Call by main()
*/
int init_status_bar() {
  status.ui_health_status = get_ui_component_by_ID(STATUS_HEALTH);
  status.ui_money_status = get_ui_component_by_ID(STATUS_MONEY);

  vec2 top_left = { -1.0, 1.0 };
  init_menu(
    top_left, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_health_status // dest
  );

  status.ui_health_status->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status.ui_health_status->text) {
    fprintf(stderr, "status.c: Failed to allocate health status buffer\n");
    return -1;
  }
  status.ui_health_status->text[0] = '\0';

  init_menu(
    top_left, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_money_status // dest
  );

  status.ui_money_status->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status.ui_money_status->text) {
    fprintf(stderr, "status.c: Failed to allocate money status buffer\n");
    return -1;
  }
  status.ui_money_status->text[0] = '\0'; // Ensures null termination
  return 0;
}

int init_status_menu() {
  status_menu.ui_status_menu = get_ui_component_by_ID(STATUS_MENU_COMPONENT);
  vec2 middle = {0, 0.5};
  init_menu(
    middle, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
    0, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_status_menu // dest
  );
  status_menu.ui_status_menu->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_status_menu->text) {
    fprintf(stderr, "status.c: Failed to allocate health status buffer\n");
    return -1;
  }
  status_menu.ui_status_menu->text[0] = '\0';

  status_menu.ui_menu_health = get_ui_component_by_ID(STATUS_MENU_HEALTH);
  // Initialize health stat
  vec2 health_position = { 0, 0.3 }; // Example position (adjust as needed)
  init_menu(
    health_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_health // dest
  );
  status_menu.ui_menu_health->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_health->text) {
    fprintf(stderr, "status.c: Failed to allocate health status buffer\n");
    return -1;
  }
  status_menu.ui_menu_health->text[0] = '\0';

  // Initialize money stat
  status_menu.ui_menu_money = get_ui_component_by_ID(STATUS_MENU_MONEY);
  vec2 money_position = { 0, 0.2 }; // Example position (adjust as needed)
  init_menu(
    money_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_money // dest
  );
  status_menu.ui_menu_money->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_money->text) {
    fprintf(stderr, "status.c: Failed to allocate money status buffer\n");
    return -1;
  }
  status_menu.ui_menu_money->text[0] = '\0';

  // Initialize attack stat
  status_menu.ui_menu_attack = get_ui_component_by_ID(STATUS_MENU_ATTACK);
  vec2 attack_position = { 0, 0.1 };
  init_menu(
    attack_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_attack // dest
  );
  status_menu.ui_menu_attack->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_attack->text) {
    fprintf(stderr, "status.c: Failed to allocate attack status buffer\n");
    return -1;
  }
  status_menu.ui_menu_attack->text[0] = '\0';

  // Initialize defense stat
  status_menu.ui_menu_defense = get_ui_component_by_ID(STATUS_MENU_DEFENSE);
  vec2 defense_position = { 0, 0.0 };
  init_menu(
    defense_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_defense // dest
  );
  status_menu.ui_menu_defense->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_defense->text) {
    fprintf(stderr, "status.c: Failed to allocate defense status buffer\n");
    return -1;
  }
  status_menu.ui_menu_defense->text[0] = '\0';

  // Initialize fire rate stat
  status_menu.ui_menu_fire = get_ui_component_by_ID(STATUS_MENU_FIRE);
  vec2 fire_position = { 0, -0.1 };
  init_menu(
    fire_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_fire // dest
  );
  status_menu.ui_menu_fire->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_fire->text) {
    fprintf(stderr, "status.c: Failed to allocate fire rate status buffer\n");
    return -1;
  }
  status_menu.ui_menu_fire->text[0] = '\0';

  // Initialize speed stat
  status_menu.ui_menu_speed = get_ui_component_by_ID(STATUS_MENU_SPEED);
  vec2 speed_position = { 0, -0.2 }; 
  init_menu(
    speed_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.65, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_LEFT, // text_anchor
    status_menu.ui_menu_speed // dest
  );
  status_menu.ui_menu_speed->text = malloc(MAX_STATUS_STR_LENGTH * sizeof(char));
  if (!status_menu.ui_menu_speed->text) {
    fprintf(stderr, "status.c: Failed to allocate speed status buffer\n");
    return -1;
  }
  status_menu.ui_menu_speed->text[0] = '\0';

  return 0;
}

/* Function used for free status bar when needed */
void free_status_bar() {
  free(status.ui_health_status->text);
  status.ui_health_status=NULL;

  free(status.ui_money_status->text);
  status.ui_money_status=NULL;
}

void free_status_menu() {
  free(status_menu.ui_status_menu->text);
  status_menu.ui_status_menu = NULL;
  free(status_menu.ui_menu_health->text);
  status_menu.ui_menu_health = NULL;
  free(status_menu.ui_menu_money->text);
  status_menu.ui_menu_money = NULL;
  free(status_menu.ui_menu_attack->text);
  status_menu.ui_menu_attack = NULL;
  free(status_menu.ui_menu_defense->text);
  status_menu.ui_menu_defense = NULL;
  free(status_menu.ui_menu_fire->text);
  status_menu.ui_menu_fire = NULL;
  free(status_menu.ui_menu_speed->text);
  status_menu.ui_menu_speed = NULL;
}

/* Update status bar for each frame */
void update_status_bar() {
  open_status_bar();
  snprintf(status.ui_health_status->text, MAX_STATUS_STR_LENGTH,
           " HEALTH %3.1f / %3.1f ", c_player.health, c_player.max_health);
  snprintf(status.ui_money_status->text, MAX_STATUS_STR_LENGTH, " G [%2d] S [%2d] C [%2d] ",
           get_player_gold(), get_player_silver(), get_player_copper());
}

void update_status_menu() {
  snprintf(status_menu.ui_status_menu->text, MAX_STATUS_STR_LENGTH,
           " Player Stats ");
  snprintf(status_menu.ui_menu_health->text, MAX_STATUS_STR_LENGTH,
           " Health : %3.1f / %3.1f ", c_player.health, c_player.max_health);
  snprintf(status_menu.ui_menu_money->text, MAX_STATUS_STR_LENGTH, " Money : G [%2d] S [%2d] C [%2d] ",
           get_player_gold(), get_player_silver(), get_player_copper());
  snprintf(status_menu.ui_menu_attack->text, MAX_STATUS_STR_LENGTH, " Attack : 10 (+0) ");
  snprintf(status_menu.ui_menu_defense->text, MAX_STATUS_STR_LENGTH, " Defense : 10 (+0) ");
  snprintf(status_menu.ui_menu_fire->text, MAX_STATUS_STR_LENGTH, " Fire Rate : %.1f ", c_player.fire_rate);
  snprintf(status_menu.ui_menu_speed->text, MAX_STATUS_STR_LENGTH, " Speed : 0.5 ");
}

/* Render status bar */
void open_status_bar() {
  if (mode == EXPLORATION) {
    status.ui_health_status->enabled = 0;
    status.ui_money_status->enabled = 1;
  } else {
    status.ui_health_status->enabled = 1;
    status.ui_money_status->enabled = 0;
  }
}

/* Derender status bar */
void close_status_bar() {
  status.ui_health_status->enabled = 0;
  status.ui_money_status->enabled = 0;
}

/* Render status bar */
void open_status_menu() {
  status_menu.ui_status_menu->enabled = 1;
  status_menu.ui_menu_health->enabled = 1;
  status_menu.ui_menu_money->enabled = 1;
  status_menu.ui_menu_attack->enabled = 1;
  status_menu.ui_menu_defense->enabled = 1;
  status_menu.ui_menu_fire->enabled = 1;
  status_menu.ui_menu_speed->enabled = 1;
  status_menu_open = 1;
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 1;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 0;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 0;
  get_ui_component_by_ID(STAT)->enabled = 0;
}

void close_status_menu() {
  status_menu.ui_status_menu->enabled = 0;
  status_menu.ui_menu_health->enabled = 0;
  status_menu.ui_menu_money->enabled = 0;
  status_menu.ui_menu_attack->enabled = 0;
  status_menu.ui_menu_defense->enabled = 0;
  status_menu.ui_menu_fire->enabled = 0;
  status_menu.ui_menu_speed->enabled = 0;
  status_menu_open = 0;
}