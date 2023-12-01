#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <menu.h>
#include <event_str.h>

/* DEFINES */
#define ENABLED (1)
#define DISABLED (0)
#define MIN_HUNGER_BEFORE_HEALTH_REDUCTION (10.0)
#define MAX_HUNGER_BEFORE_HEALTH_INCREASE (90.0)

/* GLOBALS */
float event_timer = 10.0;
extern int console_input_enabled;
float timers[NUM_TIMERS];
int event_flags[NUM_TIMERS];
extern int console_enabled;
float hunger_timer = 10.0;

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
void spawn_new_items();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
void update_timers();
float decrement_timer(float);
void item_respawn_event();
void weather_event();
/* Hunger helpers */
void start_hunger_timer();
void set_hunger_timer(float);
void reset_hunger_timer();
void decrease_hunger();
void decrease_health();
void increase_health();
void start_health_increase_timer();
void start_health_reduction_timer();
void stop_health_increase_timer();
void stop_health_reduction_timer();
void reset_health_increase_timer();
void reset_health_reduction_timer();

