#include <event.h>
/*
                                    EVENT.c
Implements the functionality for maintaining a random event timer, and spawning
random events.
*/

/* Checks time value against frame rendering speed. If hits 0, then */
/* attempt a spawn event */
void update_event_timer() {
  if ((global_time -= delta_time) <= 0.0) {
    global_time = EVENT_TIMER;
    spawn_event();
  }
}

/* Handles event spawning */
/* Enemy spawn rate chances (50%) */
void spawn_event() {
  if (rand() % 2 == 0) {
    //spawn_enemy();
    printf("SPAWN ENEMY\n");
  }
}
