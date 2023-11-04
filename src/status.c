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
  status.ui_copper_icon = get_ui_component_by_ID(STATUS_COPPER_ICON);
  status.ui_silver_icon = get_ui_component_by_ID(STATUS_SILVER_ICON);
  status.ui_gold_icon = get_ui_component_by_ID(STATUS_GOLD_ICON);
  status.ui_copper_text = get_ui_component_by_ID(STATUS_COPPER_TEXT);
  status.ui_silver_text = get_ui_component_by_ID(STATUS_SILVER_TEXT);
  status.ui_gold_text = get_ui_component_by_ID(STATUS_GOLD_TEXT);

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

  // Gold Icon
  vec2 gold_icon_pos = {
    top_left[0],
    top_left[1]
  };
  init_menu(
    gold_icon_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    1, // enabled
    1, // textured
    item_textures[GOLD_COIN], // texture
    0, // text_padding
    0, // text_scale
    0.175, // width
    0.175, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_gold_icon // dest
  );

  // Gold Text
  vec2 gold_text_pos = {
    gold_icon_pos[0]+0.175,
    top_left[1]
  };
  init_menu(
    gold_text_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Gold", // text
    1, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_gold_text // dest
  );

  // Silver Icon
  vec2 silver_icon_pos = {
    gold_text_pos[0]+0.3,
    top_left[1]
  };
  init_menu(
    silver_icon_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    1, // enabled
    1, // textured
    item_textures[SILVER_COIN], // texture
    0, // text_padding
    0, // text_scale
    0.175, // width
    0.175, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_silver_icon // dest
  );

  // Silver text
  vec2 silver_text_pos = {
    silver_icon_pos[0]+0.175,
    top_left[1]
  };
  init_menu(
    silver_text_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Silver", // text
    1, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_silver_text // dest
  );

  // Copper icon
  vec2 copper_icon_pos = {
    silver_text_pos[0]+0.3,
    top_left[1]
  };
  init_menu(
    copper_icon_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    NULL, // text
    1, // enabled
    1, // textured
    item_textures[COPPER_COIN], // texture
    0, // text_padding
    0, // text_scale
    0.175, // width
    0.175, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_copper_icon // dest
  );

  // Copper text
  vec2 copper_text_pos = {
    copper_icon_pos[0]+0.175,
    top_left[1]
  };
  init_menu(
    copper_text_pos, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "Copper", // text
    1, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_TOP_LEFT, // pivot
    T_LEFT, // text_anchor
    status.ui_copper_text // dest
  );

  status.ui_copper_text->text = malloc(MAX_STATUS_STR_LENGTH *sizeof(char));
  if (!status.ui_copper_text->text) {
    fprintf(stderr, "status.c: Failed to allocate copper status buffer\n");
    return -1;
  }
  status.ui_copper_text->text[0] = '\0'; // Ensures null termination
                                         //
  status.ui_silver_text->text = malloc(MAX_STATUS_STR_LENGTH *sizeof(char));
  if (!status.ui_silver_text->text) {
    fprintf(stderr, "status.c: Failed to allocate silver status buffer\n");
    return -1;
  }
  status.ui_silver_text->text[0] = '\0'; // Ensures null termination

  status.ui_gold_text->text = malloc(MAX_STATUS_STR_LENGTH *sizeof(char));
  if (!status.ui_gold_text->text) {
    fprintf(stderr, "status.c: Failed to allocate gold status buffer\n");
    return -1;
  }
  status.ui_gold_text->text[0] = '\0'; // Ensures null termination

  return 0;
}

/* Function used for free status bar whe needed */
void free_status_bar() {
  free(status.ui_health_status->text);
  status.ui_health_status=NULL;

  free(status.ui_copper_text->text);
  status.ui_copper_text = NULL;

  free(status.ui_silver_text->text);
  status.ui_silver_text = NULL;

  free(status.ui_gold_text->text);
  status.ui_gold_text = NULL;
}

/* Update status bar for each frame */
void update_status_bar() {
  open_status_bar();
  snprintf(status.ui_health_status->text, MAX_STATUS_STR_LENGTH,
           " HEALTH %3.1f / %3.1f ", c_player.health, c_player.max_health);
  snprintf(status.ui_copper_text->text, MAX_STATUS_STR_LENGTH, " [%2d] ",
           get_player_copper());
  snprintf(status.ui_silver_text->text, MAX_STATUS_STR_LENGTH, " S [%2d] ",
           get_player_silver());
  snprintf(status.ui_gold_text->text, MAX_STATUS_STR_LENGTH, " G [%2d] ",
           get_player_gold());
}

/* Render status bar */
void open_status_bar() {
  if (mode == EXPLORATION) {
    status.ui_health_status->enabled = 0;
    status.ui_copper_icon->enabled = 1;
    status.ui_copper_text->enabled = 1;
    status.ui_silver_icon->enabled = 1;
    status.ui_silver_text->enabled = 1;
    status.ui_gold_icon->enabled = 1;
    status.ui_gold_text->enabled = 1;
  } else {
    status.ui_health_status->enabled = 1;
    status.ui_copper_icon->enabled = 0;
    status.ui_copper_text->enabled = 0;
    status.ui_silver_icon->enabled = 0;
    status.ui_silver_text->enabled = 0;
    status.ui_gold_icon->enabled = 0;
    status.ui_gold_text->enabled = 0;
  }
}

/* Derender status bar */
void close_status_bar() {
  status.ui_health_status->enabled = 0;
  status.ui_copper_icon->enabled = 0;
  status.ui_copper_text->enabled = 0;
  status.ui_silver_icon->enabled = 0;
  status.ui_silver_text->enabled = 0;
  status.ui_gold_icon->enabled = 0;
  status.ui_gold_text->enabled = 0;
}
