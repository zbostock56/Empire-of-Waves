#include <enemy_str.h>
#include <stdio.h>

/* DEFINES */
#define EVENT_TIMER (100000000000)

/* GLOBALS */
long global_time = EVENT_TIMER;
extern int delta_time;

/* PROTOTYPES */
void update_event_timer();
void spawn_event();
