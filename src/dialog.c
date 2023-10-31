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

  dialog.mercenary_count = malloc(TEXT_BUFFER_LEN);
  if (!dialog.mercenary_count) {
    free(dialog.relationship);
    free(dialog.name);
    free(dialog.content);
    return;
  }

  dialog.type = INVALID_DIALOG;
  dialog.merchant = NULL;
  dialog.ui_text_name = get_ui_component_by_ID(DIALOG_NAME);
  dialog.ui_text_relationship = get_ui_component_by_ID(DIALOG_RELATION);
  dialog.ui_text_content = get_ui_component_by_ID(DIALOG_CONTENT);
  dialog.ui_button_buy = get_ui_component_by_ID(DIALOG_BUTTON_BUY);
  dialog.ui_button_sell = get_ui_component_by_ID(DIALOG_BUTTON_SELL);
  dialog.ui_button_trade_route = get_ui_component_by_ID(DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE);
  dialog.ui_text_schedule_trade_route_prompt = get_ui_component_by_ID(SCHEDULED_TRADE_ROUTE_PROMPT);
  dialog.ui_mercenary_buy = get_ui_component_by_ID(MERCENARY_BUY_PROMPT);

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
  vec2 relationship_position = { 1.0, 1.0 };
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
    PIVOT_TOP_RIGHT, // pivot
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
    // Trade with item update: Only use buy button as trade button
    // "1. Buy", // text
    "1. Trade", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.25, // width
    0.20, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_CENTER, // text_anchor
    dialog.ui_button_buy // dest
  );

  // Init dialog button sell
  vec2 sell_position = { -0.75, -1.0 };
  init_menu(
    sell_position, // position
    open_sell, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    // Trade with item update: Only use buy button as trade button
    "2. Sell", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.25, // width
    0.2, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_CENTER, // text_anchor
    dialog.ui_button_sell // dest
  );

  // Init dialog button establish trade route
  vec2 trade_route_button_position = { -0.75, -1.0 };
  init_menu(
    trade_route_button_position, // position
    trade_route_handler, // on_click
    NULL,
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    // Trade with item update: Only use buy button as trade button
    // "3. Establish trade route", // text
    "2. Establish trade route", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    0.5, // width
    0.2, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_CENTER, // text_anchor
    dialog.ui_button_trade_route // dest
  );

  // Init mercenary purchase button
  vec2 mercenary_buy_position = { -0.25, -1.0 };
  init_menu(
    mercenary_buy_position, // position
    purchase_mercenary_handler, // on_click
    NULL, // on_hover
    (void *) 0xBAADF00D, // on_click_args
    (void *) 0xBAADF00D, // on_hover_args
    dialog.mercenary_count, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    0.5, // text_scale
    1.25, // width
    0.2, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog.ui_mercenary_buy // dest
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
  if (dialog.name) {
    free(dialog.name);
    dialog.name = NULL;
  }

  if (dialog.relationship) {
    free(dialog.relationship);
    dialog.relationship = NULL;
  }

  if (dialog.content) {
    free(dialog.content);
    dialog.content = NULL;
  }

  if (dialog.mercenary_count) {
    free(dialog.mercenary_count);
    dialog.mercenary_count = NULL;
  }

  dialog.merchant = NULL;
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
      dialog.ui_button_trade_route->enabled = 0;
      dialog.ui_mercenary_buy->enabled = 0;
      break;
    }
    case MERCHANT_OPTION: {
      dialog.ui_text_content->enabled = 1;
      dialog.ui_text_name->enabled = 1;
      dialog.ui_text_relationship->enabled = 1;
      // Trade with item update: Only use buy button as trade button
      dialog.ui_button_buy->enabled = 1;
      dialog.ui_button_sell->enabled = 0;
      dialog.ui_button_trade_route->enabled = 1;
      dialog.ui_mercenary_buy->enabled = 1;
      break;
    }
    default: {
      dialog.ui_text_content->enabled = 1;
      dialog.ui_text_name->enabled = 1;
      dialog.ui_text_relationship->enabled = 1;
      // Trade with item update: Only use buy button as trade button
      dialog.ui_button_buy->enabled = 1;
      dialog.ui_button_sell->enabled = 0;
      dialog.ui_button_trade_route->enabled = 1;
      dialog.ui_mercenary_buy->enabled = 1;
      break;
    }
  }
}

/* Close dialog and reset schedule trade route prompt timer to 2.0 */
void close_dialog() {
  // dialog.merchant = NULL;
  dialog.ui_text_content->enabled = 0;
  dialog.ui_text_name->enabled = 0;
  dialog.ui_text_relationship->enabled = 0;
  dialog.ui_button_buy->enabled = 0;
  dialog.ui_button_sell->enabled = 0;
  dialog.ui_button_trade_route->enabled = 0;
  dialog.ui_text_schedule_trade_route_prompt->enabled = 0;
  dialog.ui_mercenary_buy->enabled = 0;
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
  content string of the dialog, do not exceed line length (20)
*/
int set_dialog(MERCHANT *merchant, T_DIALOG dialog_type, char *name,
               char *content) {
  if (strlen(name) < MAX_NAME_STR_LENGTH && strlen(content) < MAX_CONTENT_STR_LENGTH) {
    dialog.merchant = merchant;
    dialog.type = dialog_type;
    strncpy(dialog.name, name, MAX_NAME_STR_LENGTH);
    dialog.name[MAX_NAME_STR_LENGTH - 1] = '\0'; // Ensures null termination
    strncpy(dialog.content, content, MAX_CONTENT_STR_LENGTH);
    dialog.content[MAX_CONTENT_STR_LENGTH - 1] = '\0'; // Ensures null termination
    if (merchant->has_trade_route) {
      dialog.ui_button_trade_route->text = "3. Cancel trade route";
    } else {
      dialog.ui_button_trade_route->text = "3. Establish trade route";
    }
    return 1;
  }
  return 0;
}

void update_dialog_buffers() {
  if (dialog.merchant) {
    snprintf(dialog.ui_text_relationship->text, TEXT_BUFFER_LEN,
             " Relationship: %.1f ", dialog.merchant->relationship);
    snprintf(dialog.ui_mercenary_buy->text, TEXT_BUFFER_LEN,
             "3. Purchase Mercenary (%d available)",
             dialog.merchant->num_mercenaries);
  }
}
