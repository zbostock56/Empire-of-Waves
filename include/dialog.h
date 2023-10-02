#include <dialog_str.h>

DIALOG * new_dialog(char *name, char *content);
void open_dialog(DIALOG *dialog);
void close_dialog(DIALOG *dialog);
void render_dialog(GLFWwindow *window);