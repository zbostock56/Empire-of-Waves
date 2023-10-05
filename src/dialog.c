#include <ui_component.h>

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


