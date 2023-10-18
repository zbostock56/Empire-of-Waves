#include <stdio.h>
#include <stdlib.h>
#include <ui_component.h>
#include <ui_list_str.h>
#include <menu.h>
#include <globals.h>

extern UI_LIST list;
int reassignment_menu_open = 0;

/* ================= INTERNALLY DEFINED FUNCTIONS ============= */
void open_mercenary_reassignment_menu();
void close_mercenary_reassignment_menu();


/* ================= EXTERNALLY DEFINED FUNCTIONS ============= */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
void init_ui_list(UI_LIST *, unsigned int,
                  unsigned int, unsigned int,
                  char **);
void open_listing(UI_LIST *list);
void close_listings(UI_LIST *);
