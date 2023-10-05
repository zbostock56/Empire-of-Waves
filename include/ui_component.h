#include <cglm/vec2.h>
#include <string.h>
/*
                           UI_COMPONENT.h
Describes the struct representing a UI component. Can be included in any
file which enables/disabled menus, prompts, and/or buttons.
*/

typedef enum pivot {
  PIVOT_CENTER = 0,
  PIVOT_TOP = 1,
  PIVOT_BOTTOM = 2,
  PIVOT_LEFT = 3,
  PIVOT_RIGHT = 4,
  PIVOT_TOP_LEFT = 5,
  PIVOT_TOP_RIGHT = 6,
  PIVOT_BOTTOM_LEFT = 7,
  PIVOT_BOTTOM_RIGHT = 8
} PIVOT;

extern vec3 UI_PIVOT_OFFSETS[9];

typedef enum text_anchor {
  T_CENTER,
  T_LEFT,
  T_RIGHT
} TEXT_ANCHOR;

typedef struct ui_component {
  vec2 position;
  void (*on_click)(void *); // Function to call if component is clicked
                            // (NULL for no action)
  void *on_click_args; // Args passed to on_click() upon clicking component
  char *text;
  int enabled; // If 0, component will not be rendered, if 1, component will be
               // rendered
  int textured; // Whether or not the component has a background texture
                // (0 if not, nonzero if so). If not textured, the ui component
                // will have a transparent background
  unsigned int texture;
  float text_padding; // Horizontal padding for text
  float text_scale;
  float width; // 0 for dynamic width (adjusts to text width),
               // non-zero for fixed width
  float height; // O for dynamic height (adjusts to text height),
                // non-zero for fixed height
  PIVOT pivot; // Specifies "pivot" point of ui component
  TEXT_ANCHOR text_anchor; // Specifies text alignment inside ui component
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
