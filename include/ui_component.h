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

  dialog->is_open = 1;

  // Initialize the UI_COMPONENT attributes for the title box
  // dialog->ui_name.position[0] = 100; // Adjust as needed
  // dialog->ui_name.position[1] = 75;  // This is 25 units above the dialog_box. Adjust as needed.
  dialog->ui_name.text = dialog->name;

  // Initialize the UI_COMPONENT attributes for the dialog box
  // dialog->ui_content.position[0] = 100;
  // dialog->ui_content.position[1] = 100;
  // dialog->ui_content.width = 300;
  // dialog->ui_content.height = 150;
  dialog->ui_content.text = dialog->content;

  return dialog;
}

void free_dialog(DIALOG **dialog_ptr) {
  if (*dialog_ptr) {
    free((*dialog_ptr)->name);
    (*dialog_ptr)->name = NULL;

    free((*dialog_ptr)->content);
    (*dialog_ptr)->content = NULL;

    free(*dialog_ptr);
    *dialog_ptr = NULL;  // Set the external pointer to NULL
  }
}


void open_dialog(DIALOG *dialog) {
  if (dialog) {
    dialog->is_open = 1;
  }
}

void close_dialog(DIALOG *dialog) {
  if (dialog) {
    dialog->is_open = 0;
  }
}


