#include <ui_component.h>
#include <menu.h>
#include <stdio.h>
#include <globals.h>
/*
                                     MENU.c
Defines the table of all global ui components that are used across the game,
and maps each ui_component with their respective handler. menu.h provides
external references to the component table and may be included in
any file which enables/disabled ui components.
*/
/* GLOBALS DEFINES */
UI_COMPONENT ui_tab[NUM_COMPONENTS];

/*
Get UI Component by providing a enum UI_ID ui_id
Args:
UI_ID ui_id
  ID of that UI Component. e.g. TEST_MENU
*/
UI_COMPONENT * get_ui_component_by_ID(UI_ID ui_id) {
  // Check for invalid ui_id
  if (ui_id < 0 || ui_id > NUM_COMPONENTS - 2) {
    return &ui_tab[0];
  }
  return &ui_tab[ui_id + 1]; // +1 to account for INVALID_MENU being -1
}

/*
Args:
vec2 position
  Position of component anchor in screen space
  X: [-1.0, 1.0],
  Y: [-1.0, 1.0]

void (*on_click)(void *)
  Callback function for if component is clicked
  NULL if no action should be taken

void *on_click_args
  Arguments that will be passed to on_click()

char *text
  Text to be displayed over top the ui component

int enabled
  Non-zero: Component is rendered
  0: Component is not rendered

int textured
  Non-zero: Component will be rendered with a background
  texture specified by "texture"
  0: Component will be rendered with a transparent background

unsigned int texture
  Background texture of component

float text_padding
  Vertical padding of text inside ui component
  (units given in screen space)float text_scale

float text_scale
  Scale of ui component

float width
  Non-zero: Fixed width of ui component
  0: Component width will automatically scale to fit the
  width of the text

float height
  Non-zero: fixed height of ui component
  0: Component height will automatically scale to fit the
  height of the text

PIVOT pivot
  Specifies the behavior of the "pivot" of the ui component
  See PIVOT enum in ui_component.h

TEXT_ANCHOR text_anchor
  Specifies the alignment of the text
  See TEXT_ANCHOR enum in ui_component.h

UI_COMPONENT *dest
  Destination component
*/
void init_menu(vec2 position, void (*on_click)(void *), void *on_click_args,
               char *text, int enabled, int textured, unsigned int texture,
               float text_padding, float text_scale, float width, float height,
               PIVOT pivot, TEXT_ANCHOR text_anchor, UI_COMPONENT *dest) {
  glm_vec2_copy(position, dest->position);
  dest->on_click = on_click;
  dest->on_click_args = on_click_args;
  dest->text = text;
  // sprintf(dest->text, "%s", text);
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

/* Init menus including TEST_MENU, EMBARK_PROMPT, and INTERACT_PROMPT */
void init_menus() {
  // Populate ui_tab
  // TEMPORARY TEST UI COMPONENT -- TO BE DELETED
  vec2 test_position = { -1.0, 0.0 };
  init_menu(
    test_position, // position
    test_callback, // on_click
    (void *) 0xBAADF00D, // on_click_args
    "", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    1.0, // text_scale
    1.0, // width
    1.0, // height
    PIVOT_LEFT, // pivot
    T_RIGHT, // text_anchor
    get_ui_component_by_ID(TEST_MENU) // dest
  );

  // Embark/Disembark prompt
  vec2 prompt_pos = { 0.0, -0.15 };
  init_menu(
    prompt_pos,
    NULL, NULL,
    "Press 'e' to embark",
    0,
    0,
    0,
    0.0,
    0.5,
    0.0,
    0.0,
    PIVOT_CENTER,
    T_CENTER,
    get_ui_component_by_ID(EMBARK_PROMPT)
  );

  // Interaction prompt
  init_menu(
    prompt_pos,
    NULL, NULL,
    "Press 'e' to interact",
    0,
    0,
    0,
    0.0,
    0.5,
    0.0,
    0.0,
    PIVOT_CENTER,
    T_CENTER,
    get_ui_component_by_ID(INTERACT_PROMPT)
  );

/* Past logic, commented by Zack */
#if 0
  // Init dialog menu
  init_dialog();
  // Init trade menu
  init_trade();
  // Give Init Money
  e_player.money = 100;
  // Init status bar
  init_status_bar();
  open_status_bar();
#endif
}
