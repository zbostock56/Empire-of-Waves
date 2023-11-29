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
STATUS_MENU status_menu;
int status_menu_open = 0;
/* =================== INTERNALLY DEFINED FUNCTIONS ================ */
int init_status_bar();
void free_status();
void update_status_bar();
void open_status_bar();
void close_status_bar();
int init_status_menu();
void free_status_menu();
void update_status_menu();
void update_buff_list();
void increment_buff();
void open_status_menu();
void close_status_menu();

/* =================== EXTERNALLY DEFINED FUNCTIONS =============== */
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
float decrement_timer(float);