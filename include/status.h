#include <ui_component.h>
#include <merchant_str.h>
#include <dialog_str.h>
#include <menu.h>
#include <status_str.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>
#include <player.h>

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
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
