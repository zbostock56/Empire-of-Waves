#include <menu.h>
#include <stdio.h>
/*
                                     MENU.c
Defines the table of all global ui components that are used across the game,
and maps each ui_component with their respective handler. menu.h provides
external references to the component table and may be included in
any file which enables/disabled ui components.
*/

// Init global variables
DIALOG * dialog;

// UI component table definition
UI_COMPONENT ui_tab[NUM_COMPONENTS];

UI_COMPONENT * get_ui_component_by_ID(UI_ID ui_id) {
  // Check for invalid ui_id
  if (ui_id < TEST_MENU || ui_id > DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE) {
    return &ui_tab[0];
  }
  return &ui_tab[ui_id + 1]; // +1 to account for INVALID_MENU being -1
}

void init_menu(
  vec2 position, // Position of component anchor in screen space
                 // X: [-1.0, 1.0],
                 // Y: [-1.0, 1.0]
  void (*on_click)(void *), // Callback function for if component is clicked
                            // NULL if no action should be taken
  void *on_click_args, // Arguments that will be passed to on_click()
  char *text, // Text to be displayed over top the ui component
  int enabled, // Non-zero: Component is rendered
                // 0: Component is not rendered
  int textured, // Non-zero: Component will be rendered with a background
                // texture specified by "texture"
                // 0: Component will be rendered with a transparent
                // background
  unsigned int texture, // Background texture of component
  float text_padding, // Vertical padding of text inside ui component
                      // (units given in screen space)
  float text_scale, // Scale of ui component
  float width, // Non-zero: Fixed width of ui component
               // 0: Component width will automatically scale to fit the
               // width of the text
  float height, // Non-zero: fixed height of ui component
                // 0: Component height will automatically scale to fit the
                // height of the text
  PIVOT pivot, // Specifies the behavior of the "pivot" of the ui component
               // See PIVOT enum in ui_component.h
  TEXT_ANCHOR text_anchor, // Specifies the alignment of the text
                           // See TEXT_ANCHOR enum in ui_component.h
  UI_COMPONENT *dest
) {
  glm_vec2_copy(position, dest->position);
  dest->on_click = on_click;
  dest->on_click_args = on_click_args;
  dest->text = text;
  dest->enabled = enabled;
  dest->textured = textured;
  dest->texture = texture;
  dest->text_padding = text_padding;
  dest->text_scale = text_scale;
  dest->width = width;
  dest->height = height;
  dest->pivot = pivot;
  dest->text_anchor = text_anchor;
}

// TEMPORARY TEST CALLBACK -- TO BE DELETED
void test_callback(void *args) {
  get_ui_component_by_ID(TEST_MENU)->text = "HIT!";
}

void init_menus() {
  // Populate ui_tab

  // TEMPORARY TEST UI COMPONENT -- TO BE DELETED
  vec2 test_position = { 0.0, -0.75 };
  init_menu(
    test_position, // position
    test_callback, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "Hello, this is a merchant!", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    2.0, // text_scale
    0, // width
    0, // height
    PIVOT_CENTER, // pivot
    T_RIGHT, // text_anchor
    get_ui_component_by_ID(TEST_MENU) // dest
  );

  // Init Dialog menu
  dialog = new_dialog("", "");
  // Dialog merchant menu mesting
  set_dialog(MERCHANT_OPTION, "Merchant", "Hail, Captain! What brings you to my humble stall");
  open_dialog();
}


DIALOG * new_dialog(char *name, char *content) {
  DIALOG *dialog = malloc(sizeof(DIALOG));
  if (!dialog) {
    // Handle memory allocation failure, e.g., return NULL or exit
    return NULL;
  }
  
  dialog->name = malloc(MAX_NAME * sizeof(char));
  if (!dialog->name) {
    free(dialog);
    return NULL;
  }
  strncpy(dialog->name, name, MAX_NAME);
  dialog->name[MAX_NAME - 1] = '\0'; // Ensures null termination

  dialog->content = malloc(MAX_CONTENT * sizeof(char));
  if (!dialog->content) {
      free(dialog->name);
      free(dialog);
      return NULL;
  }
  strncpy(dialog->content, content, MAX_CONTENT);
  dialog->content[MAX_CONTENT - 1] = '\0'; // Ensures null termination

  dialog->type = DEFAULT;
  dialog->ui_text_name = get_ui_component_by_ID(DIALOG_NAME);
  dialog->ui_text_content = get_ui_component_by_ID(DIALOG_CONTENT);
  dialog->ui_button_buy = get_ui_component_by_ID(DIALOG_BUTTON_BUY);
  dialog->ui_button_sell = get_ui_component_by_ID(DIALOG_BUTTON_SELL);
  dialog->ui_button_establish_trade_route = get_ui_component_by_ID(DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE);

  // Init content
  vec2 content_position = { -1.0, -0.75 };
  init_menu(
    content_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    dialog->content, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // height
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_text_content // dest
  );

  // Init name
  vec2 name_position = { -1.0, -0.5 };
  init_menu(
    name_position, // position
    NULL, // on_click
    (void *) 0xBAADF00D, // on_click_args
    dialog->name, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_text_name // dest
  );

  // Init dialog button buy
  vec2 buy_position = { -1.0, -1.0 };
  init_menu(
    buy_position, // position
    open_buy, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "1. Buy", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0.5, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_buy // dest
  );

  // Init dialog button sell
  vec2 sell_position = { -0.5, -1.0 };
  init_menu(
    sell_position, // position
    open_sell, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "2. Sell", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    0.5, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_sell // dest
  );

  // Init dialog button establish trade route
  vec2 establish_trade_route_position = { 0.0, -1.0 };
  init_menu(
    establish_trade_route_position, // position
    open_establish_trade_route, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "3. Establish trade route", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.5, // text_scale
    1.0, // width
    0, // heights
    PIVOT_BOTTOM_LEFT, // pivot
    T_LEFT, // text_anchor
    dialog->ui_button_establish_trade_route // dest
  );

  return dialog;
}

void free_dialog() {
  if (dialog) {
    free(dialog->name);
    dialog->name = NULL;

    free(dialog->content);
    dialog->content = NULL;

    free(dialog);
    dialog = NULL;  // Set the global dialog to NULL
  }
}

void open_dialog() {
  if (dialog) {
    switch (dialog->type) {
      case TALK: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_button_buy->enabled = 0;
        dialog->ui_button_sell->enabled = 0;
        dialog->ui_button_establish_trade_route->enabled = 0;
        break;
      }
      case MERCHANT_OPTION: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_button_buy->enabled = 1;
        dialog->ui_button_sell->enabled = 1;
        dialog->ui_button_establish_trade_route->enabled = 1;
        break;
      }
      default: {
        dialog->ui_text_content->enabled = 1;
        dialog->ui_text_name->enabled = 1;
        dialog->ui_button_buy->enabled = 1;
        dialog->ui_button_sell->enabled = 1;
        dialog->ui_button_establish_trade_route->enabled = 1;
        break;
      }
    }
  }
}

void close_dialog() {
  if (dialog) {
    dialog->ui_text_content->enabled = 0;
    dialog->ui_text_name->enabled = 0;
    dialog->ui_button_buy->enabled = 0;
    dialog->ui_button_sell->enabled = 0;
    dialog->ui_button_establish_trade_route->enabled = 0;
  }
}

int set_dialog(T_DIALOG dialog_type, char *name, char *content) {
  if (dialog && strlen(name) < MAX_NAME && strlen(content) < MAX_CONTENT) {
    dialog->type = dialog_type;
    strncpy(dialog->name, name, MAX_NAME);
    dialog->name[MAX_NAME - 1] = '\0'; // Ensures null termination
    strncpy(dialog->content, content, MAX_CONTENT);
    dialog->content[MAX_CONTENT - 1] = '\0'; // Ensures null termination
    return 1;
  }
  return 0;
}

void open_buy() {
  get_ui_component_by_ID(DIALOG_BUTTON_BUY)->text = "HIT!";
}

void close_buy() {

}

void open_sell() {
  get_ui_component_by_ID(DIALOG_BUTTON_SELL)->text = "HIT!";
}

void close_sell() {

}

void open_establish_trade_route() {
  get_ui_component_by_ID(DIALOG_BUTTON_ESTABLISH_TRADE_ROUTE)->text = "HIT!";
}

void close_establish_trade_route() {

}
