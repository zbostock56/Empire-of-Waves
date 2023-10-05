#include <menu.h>
#include <stdio.h>
/*
                                     MENU.c
Defines the table of all global ui components that are used across the game,
and maps each ui_component with their respective handler. menu.h provides
external references to the component table and may be included in
any file which enables/disabled ui components.
*/

// UI component table definition
UI_COMPONENT ui_tab[NUM_COMPONENTS] = {
  /*
  {
    position (vec2)
    on_click (void (*)(void *))
    text (char *)
    enabled (int)
    textured (int)
    texture (unsigned int)
    text_len (unsigned int)
    width (float)
    height (float)
  }
  */
};

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
  ui_tab[0].text = "HIT!";
}

void init_menus() {
  // Populate ui_tab

  // TEMPORARY TEST UI COMPONENT -- TO BE DELETED
  vec2 test_position = { 0.0, -0.75 };
  init_menu(
      test_position,
      test_callback,
      (void *) 0xBAADF00D,
      "Hello, this is a merchant!",
      1,
      1,
      0,
      0.01,
      2.0,
      0.75,
      0.25,
      PIVOT_CENTER,
      T_RIGHT,
      ui_tab);
}

