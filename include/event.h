#include <enemy_str.h>
#include <stdio.h>

/* DEFINES */
#define EVENT_TIMER (2.0)

/* GLOBALS */
float global_time = EVENT_TIMER;
extern float delta_time;

/* PROTOTYPES */
void update_event_timer();
void spawn_event();
