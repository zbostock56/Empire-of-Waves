#include <stdio.h>
#include <ui_component.h>
#include <menu.h>
#include <globals.h>

#define INVASION_BUFF_SIZE (64)
#define RANSOM_BUFF_SIZE (32)

//static float val_stolen = 0.0;
//static unsigned int num_enemies = 0;
//static char invasion_buffer[INVASION_BUFF_SIZE];
static char ransom_buffer[RANSOM_BUFF_SIZE];

/* ================= INTERNALLY DEFINED FUNCTIONS ================= */

void pay_ransom();

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */

void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
float calc_ransom_price();
void remove_money(I_SLOT *, unsigned int);
void coalesce_currency(I_SLOT *, unsigned int);
void give_player_copper(unsigned int);
unsigned int get_player_money();
void set_prompt(const char *);
void chunk_to_world(ivec2, vec2, vec2);

