#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <menu.h>
#include <event_str.h>

/* DEFINES */
#define EVENT_TIMER (10.0)
#define ENABLED (1)
#define DISABLED (0)

/* GLOBALS */
extern int console_input_enabled;
float timers[NUM_TIMERS];
int event_flags[NUM_TIMERS];
extern int console_enabled;

/* ================ EXTERNALLY DEFINED FUNCTIONS ================== */
void spawn_enemy();
int invade_home_island();
void reset_console_error();
void set_prompt(const char *);
void close_prompt();
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
float calc_stealing_interval();
void steal_item(CONTAINER *);

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
void update_timers();
float decrement_timer(float);
