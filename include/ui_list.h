#include <stdio.h>
#include <ui_component.h>
#include <menu.h>
#include <ui_list_str.h>

/* GLOBALS */
UI_LIST list;
/* Array of pointers to the text descriptions */
char *listing_strings[MAX_PER_PAGE];
extern unsigned int num_trade_ships;

/* ================= INTERNALLY DEFINED FUNCTIONS ============= */
void init_ui_list(UI_LIST *, unsigned int,
                  unsigned int, unsigned int,
                  char **);
void open_listing(UI_LIST *list);
void close_listings(UI_LIST *);
void scroll_left(UI_LIST *list);
void scroll_right(UI_LIST *list);
/* ================= EXTERNALLY DEFINED FUNCTIONS ============= */
UI_COMPONENT *get_ui_component_by_ID(UI_ID ui_id);
void init_menu(vec2 position, void (*on_click)(void *), void *on_click_args,
               char *text, int enabled, int textured, unsigned int texture,
               float text_padding, float text_scale, float width, float height,
               PIVOT pivot, TEXT_ANCHOR text_anchor, UI_COMPONENT *dest);
