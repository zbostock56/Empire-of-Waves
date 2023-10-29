#include <stdio.h>
#include <ui_component.h>
#include <menu.h>
#include <ui_list_str.h>
#include <globals.h>

/* GLOBALS */
UI_LIST list;
/* Array of pointers to the text descriptions */
char *listing_strings[MAX_PER_PAGE];
extern unsigned int num_trade_ships;
extern int reassignment_menu_open;

/* ================= INTERNALLY DEFINED FUNCTIONS ============= */
void init_ui_list(UI_LIST *, unsigned int,
                  unsigned int, unsigned int,
                  char **);
void open_listing(UI_LIST *list);
void close_listings(UI_LIST *);
void scroll_left(UI_LIST *list);
void scroll_right(UI_LIST *list);
void init_ui_lists();
void select_listing_dispatcher();
/* ================= EXTERNALLY DEFINED FUNCTIONS ============= */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
void check_available_mercenaries();
void mercenary_reassignment_selection(int);

