#include <dialog.h>
/*
                                    Dialog.c
Implements the functionality for dialog, itegrate with UI components
*/

/*  Init global dialog, call by main() */
void init_dialog() {
  /* Alloc size for dialog name string*/
  dialog.name = malloc(MAX_NAME_STR_LENGTH * sizeof(char));
  if (!dialog.name) {
    return;
  }
  dialog.name[0] = '\0'; // Ensures null termination

  /* Alloc size for dialog relationship string*/
  dialog.relationship = malloc(TEXT_BUFFER_LEN * sizeof(char));
  if (!dialog.relationship) {
    free(dialog.name);
    return;
  }
  dialog.relationship[0] = '\0';

  dialog.content = malloc(MAX_CONTENT_STR_LENGTH * sizeof(char));
  if (!dialog.content) {
    free(dialog.relationship);
    free(dialog.name);
    return;
  }
  dialog.content[0] = '\0'; // Ensures null termination

  dialog.type = INVALID_DIALOG;
  dialog.ui_text_name = get_ui_component_by_ID(DIALOG_NAME);
  dialog.ui_text_relationship = get_ui_component_by_ID(DIALOG_RELATION);
  dialog.ui_text_content = get_ui_component_by_ID(DIALOG_CONTENT);
  dialog.ui_button_buy = get_ui_component_by_ID(DIALOG_BUTTON_BUY);
  dialog.ui_button_sell = get_ui_component_by_ID(DIALOG_BUTTON_SELL);
  dialog.ui_button_establish_trade_route = get_ui_component_by_ID(DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE);
  dialog.ui_text_schedule_trade_route_prompt = get_ui_component_by_ID(SCHEDULED_TRADE_ROUTE_PROMPT);

  // Init content
  vec2 content_position = { -1.0, -0.8 };
  init_menu(
    content_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    dialog.content, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    2.0, // width
    0.25, // height
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_text_content // dest
  );

  // Init name
  vec2 name_position = { -1.0, -0.55 };
  init_menu(
    name_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    dialog.name, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_text_name // dest
  );

  // Init relationship
  vec2 relationship_position = { 1.0, -0.55 };
  init_menu(
    relationship_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    dialog.relationship, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0, // width
    0, // height
    PIVOT_BOTTOM_RIGHT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_text_relationship // dest
  );

  // Init dialog button buy
  vec2 buy_position = { -1.0, -1.0 };
  init_menu(
    buy_position, // position
    open_buy, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "1. Buy", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.20, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_button_buy // dest
  );

  // Init dialog button sell
  vec2 sell_position = { -0.5, -1.0 };
  init_menu(
    sell_position, // position
    open_sell, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "2. Sell", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.20, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_button_sell // dest
  );

  // Init dialog button establish trade route
  vec2 establish_trade_route_position = { 0.0, -1.0 };
  init_menu(
    establish_trade_route_position, // position
    open_establish_trade_route, // on_click
    NULL,
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "3. Establish trade route", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    1.0, // width
    0.25, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_button_establish_trade_route // dest
  );

  // Init schdule trade route prompt
  vec2 establish_trade_route_prompt_position = { 0.0, 0.0 };
  init_menu(
    establish_trade_route_prompt_position, // position
    NULL, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    "TRADE_ROUTE_PROMPT", // text
    0, // enabled
    1, // textured
    0, // texture
    0.1, // text_padding
    0.5, // text_scale
    0, // width
    0, // heights
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    dialog.ui_text_schedule_trade_route_prompt // dest
  );
}

/* Free Dialog Allocated Spaces */
void free_dialog() {
  free(dialog.name);
  dialog.name = NULL;

  free(dialog.relationship);
  dialog.relationship = NULL;

  free(dialog.content);
  dialog.content = NULL;
}

/* Open Dialog UI based on dialog type */
void open_dialog() {
  switch (dialog.type) {
    case TALK: {
      dialog.ui_text_content->enabled = 1;
      dialog.ui_text_name->enabled = 1;
      dialog.ui_text_relationship->enabled = 1;
      dialog.ui_button_buy->enabled = 0;
      dialog.ui_button_sell->enabled = 0;
      dialog.ui_button_establish_trade_route->enabled = 0;
      break;
    }
    case MERCHANT_OPTION: {
      dialog.ui_text_content->enabled = 1;
      dialog.ui_text_name->enabled = 1;
      dialog.ui_text_relationship->enabled = 1;
      dialog.ui_button_buy->enabled = 1;
      dialog.ui_button_sell->enabled = 1;
      dialog.ui_button_establish_trade_route->enabled = 1;
      break;
    }
    default: {
      dialog.ui_text_content->enabled = 1;
      dialog.ui_text_name->enabled = 1;
      dialog.ui_text_relationship->enabled = 1;
      dialog.ui_button_buy->enabled = 1;
      dialog.ui_button_sell->enabled = 1;
      dialog.ui_button_establish_trade_route->enabled = 1;
      break;
    }
  }
}

/* Close dialog and reset schedule trade route prompt timer to 2.0 */
void close_dialog() {
  dialog.ui_text_content->enabled = 0;
  dialog.ui_text_name->enabled = 0;
  dialog.ui_text_relationship->enabled = 0;
  dialog.ui_button_buy->enabled = 0;
  dialog.ui_button_sell->enabled = 0;
  dialog.ui_button_establish_trade_route->enabled = 0;
  dialog.ui_text_schedule_trade_route_prompt->enabled = 0;
  time_schdule_trade_toute_prompt = 2.0;
}

/*
Set global dialog, call when change dialog
Args:
T_DIALOG dialog type
  Decided the type of dialog when call by open_dialog()
char *name 
  name string of the dialog, represent the who says the content
char *content
  content string of the dialog, do not exceed line length (16)
*/
int set_dialog(T_DIALOG dialog_type, char *name, char *content) {
  if (strlen(name) < MAX_NAME_STR_LENGTH && strlen(content) < MAX_CONTENT_STR_LENGTH) {
    dialog.type = dialog_type;
    strncpy(dialog.name, name, MAX_NAME_STR_LENGTH);
    dialog.name[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination
    strncpy(dialog.content, content, MAX_CONTENT_STR_LENGTH);
    dialog.content[MAX_CONTENT_STR_LENGTH - 1] = '\0'; // Ensures null termination
    return 1;
  }
  return 0;
}
