#include <merchant_str.h>
#include <ui_component.h>
#include <dialog_str.h>
#include <menu.h>

/*
                                   DIALOG
Implements the functionality for opening and closing a dialog menu. Could be
used for conversation with merchants or other places that need a dialog box.
*/


/* GLOBALS */
DIALOG dialog;
float time_schdule_trade_toute_prompt;

/* ================= INTERNALLY DEFINED FUNCTIONS ================= */
void init_dialog();
void free_dialog();
void open_dialog();
void close_dialog();
int set_dialog(T_DIALOG, char *, char *);

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
void open_buy();
void open_sell();
void open_establish_trade_route();
