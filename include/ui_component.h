#include <cglm/vec2.h>
#include <string.h>
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

/*
                                   dialog
Implements the functionality for opening and closing a dialog box. Could be 
used for conversation with merchants or other places that need a dialog box.
*/

#define MAX_NAME (16) // Max string length of name
#define MAX_CONTENT (256) // Max string length of content
#define MAX_DIALOGS (16) // Max number of dialogs

typedef struct dialog {
    char *name;
    char *content;
    int is_open;
    UI_COMPONENT ui_name;
    UI_COMPONENT ui_content;
} DIALOG;

DIALOG * new_dialog(char *name, char *content);
// void render_dialog(GLFWwindow *window, DIALOG *dialog);
void free_dialog(DIALOG **dialog_ptr);
void open_dialog(DIALOG *dialog);
void close_dialog(DIALOG *dialog);
