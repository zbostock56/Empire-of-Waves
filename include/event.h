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
void reset_console_error();
void reset_merc_trade_error();
void clear_plundered_trade_ship_prompt();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
void update_timers();
