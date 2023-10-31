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
    if (mode == EXPLORATION && e_player.embarked == 1) {
      spawn_event();
    }
  }
}

/* Handles event spawning */
/* Enemy spawn rate chances (50%) */
void spawn_event() {
  if (rand() % 2 == 0) {
    spawn_enemy();
  }
}

void init_timers() {
  for (int i = 0; i < NUM_TIMERS; i++) {
    timers[i] = 0.0;
    event_flags[i] = 0;
  }
}

void update_timers() {
  if (console_input_enabled) {
    timers[CONS_CURSOR] -= delta_time;
    if (timers[CONS_CURSOR] <= 0.0 && event_flags[CONS_CURSOR]) {
      timers[CONS_CURSOR] = C_CURSOR_TIME;
      event_flags[CONS_CURSOR] = DISABLED;
    } else if (timers[CONS_CURSOR] <= 0.0 && !event_flags[CONS_CURSOR]) {
      timers[CONS_CURSOR] = C_CURSOR_TIME;
      event_flags[CONS_CURSOR] = ENABLED;
    }
  }
  if (event_flags[CONS_ERROR]) {
    timers[CONS_ERROR] -= delta_time;
    if (timers[CONS_ERROR] <= 0.0) {
      event_flags[CONS_ERROR] = 0;
      timers[CONS_ERROR] = C_ERROR_TIME;
      reset_console_error();
    }
  }
  if (event_flags[RELATIONSHIP_TOO_LOW]) {
    timers[RELATIONSHIP_TOO_LOW] -= delta_time;
    if (timers[RELATIONSHIP_TOO_LOW] <= 0.0) {
      event_flags[RELATIONSHIP_TOO_LOW] = 0;
      timers[RELATIONSHIP_TOO_LOW] = TRADE_ERROR_TIME;
      reset_merc_trade_error();
    }
  }
}
