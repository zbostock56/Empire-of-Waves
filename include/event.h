#include <enemy_str.h>
#include <stdio.h>

/* DEFINES */
#define EVENT_TIMER (2.0)

/* GLOBALS */
float global_time = EVENT_TIMER;
extern float delta_time;

/* ================ EXTERNALLY DEFINED FUNCTIONS ================== */
void spawn_enemy();

/* ================ INTERNALLY DEFINED FUNCTIONS ================== */
void update_event_timer();
void spawn_event();
