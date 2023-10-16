#include <ui_component.h>
#include <merchant_str.h>
#include <dialog_str.h>
#include <menu.h>
#include <status_str.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>

/*
                                   STATUS
  Implements the functionality for shows player status.
*/

/* GLOABALS */
STATUS status;

/* =================== INTERNALLY DEFINED FUNCTIONS ================ */
void init_status_bar();
void free_status();
void update_status_bar();
void open_status_bar();
void close_status_bar();

/* =================== EXTERNALLY DEFINED FUNCTIONS =============== */
void init_menu(vec2 position, void (*on_click)(void *), void *on_click_args,
               char *text, int enabled, int textured, unsigned int texture,
               float text_padding, float text_scale, float width, float height,
               PIVOT pivot, TEXT_ANCHOR text_anchor, UI_COMPONENT *dest);
