#include <stdio.h>
#include <ui_component.h>
#include <menu.h>

#define INVASION_BUFF_SIZE (64)

//static float val_stolen = 0.0;
//static unsigned int num_enemies = 0;
static char invasion_buffer[INVASION_BUFF_SIZE];

/* ================= INTERNALLY DEFINED FUNCTIONS ================= */

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */

void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);

