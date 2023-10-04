#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

/* DEFINES */
#define EVENT_TIMER (0.10)

/* GLOBALS */
float global_time = EVENT_TIMER;

/* ================ EXTERNALLY DEFINED FUNCTIONS ================== */
void spawn_enemy();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
