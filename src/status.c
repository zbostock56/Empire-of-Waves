#include <status.h>

/*
                                   STATUS
Implements the functionality for shows player status.
*/

/*
Init status bar
Call by main()
*/
void init_status_bar() {
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
    return;
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
    return;
  }
  status.ui_money_status->text[0] = '\0'; // Ensures null termination
}

/* Function used for free status bar whe needed */
void free_status_bar() {
  free(status.ui_health_status);
  status.ui_health_status=NULL;

  free(status.ui_money_status);
  status.ui_money_status=NULL;
}

/* Update status bar for each frame */
void update_status_bar() {
  open_status_bar();
  snprintf(status.ui_health_status->text, MAX_STATUS_STR_LENGTH,
           " HEALTH %3.1f / %3.1f ", c_player.health, c_player.max_health);
  snprintf(status.ui_money_status->text, MAX_STATUS_STR_LENGTH, " MONEY %4d ",
           e_player.money);
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
