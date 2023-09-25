#include <menu.h>
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
