#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <ui_component.h>
#include <menu.h>

/* DEFINES */
#define EVENT_TIMER (10.0)

/* GLOBALS */
extern float console_cursor_interval;
extern float console_error_interval;
extern int console_enabled;
extern int cursor_enabled;
extern int console_error;

/* ================ EXTERNALLY DEFINED FUNCTIONS ================== */
void spawn_enemy();
void reset_console_error();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
void update_timers();
