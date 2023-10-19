#include <stdio.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

/* DEFINES */
#define EVENT_TIMER (10.0)

/* ================ EXTERNALLY DEFINED FUNCTIONS ================== */
void spawn_enemy();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
