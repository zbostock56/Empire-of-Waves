#include <stdio.h>
#include <ui_component.h>
#include <menu.h>
#include <globals.h>

extern int RES_X;
extern int RES_Y;
extern int holding_shift;

static CONTAINER c1;
static CONTAINER c2;
static int c1_page = 0;
static int c2_page = 0;

/* ================= INTERNALLY DEFINED FUNCTIONS ================= */
void open_container(CONTAINER, CONTAINER);
void refresh_containers();
void c1_slot_on_click(void *);
void c2_slot_on_click(void *);
void container_next(void *);
void container_prev(void *);
void move_c1_item(void *);
void move_c2_item(void *);

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
int move_item(I_SLOT *, I_SLOT *);
