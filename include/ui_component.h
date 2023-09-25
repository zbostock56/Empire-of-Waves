#include <cglm/vec2.h>
/*
                           UI_COMPONENT.h
Describes the struct representing a UI component. Can be included in any
file which enables/disabled menus, prompts, and/or buttons.
*/

typedef struct ui_component {
  vec2 position;
  void (*on_click)(void *);
  char *text;
  int enabled;
  int textured;
  unsigned int texture;
  unsigned int text_len;
  float width;
  float height;
} UI_COMPONENT;
