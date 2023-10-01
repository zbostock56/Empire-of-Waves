#include <dialog.h>
#include <stdio.h>
#include <render.h>

/*
                                   dialog.c
Implements the functionality for opening and closing a dialog box. Could be 
used for conversation with merchants or other places that need a dialog box.
*/

DIALOG * new_dialog(char *name, char *content) {
    DIALOG *dialog = NULL;
    dialog->name = name;
    dialog->content = content;
    return dialog;
}

void open_dialog(DIALOG *dialog) {
    mat4 text_model_dialog_name = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(text_model_dialog_name, -1.0);
    glm_translate_y(text_model_dialog_name, 0.75);
    glm_rotate_z(text_model_dialog_name, glm_rad(90.0), text_model_dialog_name);
    glm_rotate_x(text_model_dialog_name, glm_rad(90.0), text_model_dialog_name);
    glm_scale_uni(text_model_dialog_name, 2.0);
    render_text(dialog->name, text_model_dialog_name);

    mat4 text_model_dialog_content = GLM_MAT4_IDENTITY_INIT;
    glm_translate_z(text_model_dialog_content, -1.0);
    glm_translate_y(text_model_dialog_content, 0.75);
    glm_rotate_z(text_model_dialog_content, glm_rad(90.0), text_model_dialog_content);
    glm_rotate_x(text_model_dialog_content, glm_rad(90.0), text_model_dialog_content);
    glm_scale_uni(text_model_dialog_content, 2.0);
    render_text(dialog->content, text_model_dialog_content);
};

void close_dialog(DIALOG *dialog) {

// Derender dialog assets for current window

};
