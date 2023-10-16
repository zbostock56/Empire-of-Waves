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
int set_dialog(T_DIALOG, MERCHANT *, char *, char *);

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */
void init_menu(vec2 position, void (*on_click)(void *), void *on_click_args,
               char *text, int enabled, int textured, unsigned int texture,
               float text_padding, float text_scale, float width, float height,
               PIVOT pivot, TEXT_ANCHOR text_anchor, UI_COMPONENT *dest);
void open_buy();
void open_sell();
void open_establish_trade_route();
