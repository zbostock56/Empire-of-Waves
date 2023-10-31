#include <stdio.h>
#include <stdlib.h>
#include <ui_component.h>
#include <ui_list_str.h>
#include <merchant_str.h>
#include <dialog_str.h>
#include <menu.h>
#include <globals.h>

#define AVAIL_MERC_BUFF_LEN (32)
#define TOT_LISTING_NUM (17)
#define MY_SHIP_MERC_LEN (48)
#define MIN_MERC_THRESHOLD (30.0)

extern UI_LIST list;
extern DIALOG dialog;
extern MERCHANT *cur_merchant;
int reassignment_menu_open = 0;
char avail_mercenaries[AVAIL_MERC_BUFF_LEN];
int mercenary_listing_selected[TOT_LISTING_NUM];
char **mercenary_reassign_listings;
char my_ship_mercenaries[MY_SHIP_MERC_LEN];

/* ================= INTERNALLY DEFINED FUNCTIONS ============= */
void open_mercenary_reassignment_menu();
void close_mercenary_reassignment_menu();
void update_available_mercenaries();
void check_available_mercenaries();
void mercenary_reassignment_selection();
void mercenary_reassign(void *);
void update_mercenary_reassignment();
void mercenary_reassign_my_ship(long);

/* ================= EXTERNALLY DEFINED FUNCTIONS ============= */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
void init_ui_list(UI_LIST *, unsigned int,
                  unsigned int, unsigned int,
                  char **);
void open_listing(UI_LIST *list);
void close_listings(UI_LIST *);
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
void select_listing_dispatcher();

