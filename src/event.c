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
    if (mode == EXPLORATION) {
      item_respawn_event();
      weather_event();
    }
  }
}

/*
  Helper routine to spawn items
  on islands which have already had
  items spawned on them
*/
void item_respawn_event() {
  if (rand() % 5 == 0) {
    spawn_new_items();
  }
}

/* Handles event spawning */
/*
  Enemy spawn rate chances (50%)
  Invasion spawn rate chances (10%)
*/
void spawn_event() {
  unsigned int event = rand() % 100;
  int status = 0;
  if (event < 50) {
    spawn_enemy();
  } else if (event < 60) {
    status = invade_home_island();
    if (status) {
      exit(1);
    }
  }
}

void weather_event() {
  if (rand() % 5 == 0 && weather == CLEAR) {
    weather = FOG;
    event_flags[WEATHER] = ENABLED;
    timers[WEATHER] = WEATHER_TIME;
  }
}

void init_timers() {
  for (int i = 0; i < NUM_TIMERS; i++) {
    timers[i] = 0.0;
    event_flags[i] = 0;
  }
}

/*
  Helper routine to decrement time from active timers
  and activate their respective events if they go off
*/
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
      event_flags[CONS_ERROR] = DISABLED;
      timers[CONS_ERROR] = C_ERROR_TIME;
      reset_console_error();
    }
  }
  if (event_flags[GENERAL_PROMPT]) {
    timers[GENERAL_PROMPT] -= delta_time;
    if (timers[GENERAL_PROMPT] <= 0.0) {
      event_flags[GENERAL_PROMPT] = DISABLED;
      timers[GENERAL_PROMPT] = GENERAL_PROMPT_TIME;
      close_prompt();
    }
  }
  if (mode == EXPLORATION) {
    TRADE_SHIP *cur_ship = NULL;
    for (int i = 0; i < num_trade_ships; i++) {
      cur_ship = trade_ships + i;
      if (cur_ship->death_animation > 0.0) {
        cur_ship->death_animation = decrement_timer(cur_ship->death_animation);
      }
    }

    if (event_flags[WEATHER]) {
      timers[WEATHER] = decrement_timer(timers[STEALING_TIMER]);
      if (timers[WEATHER] == 0.0) {
        event_flags[WEATHER] = DISABLED;
        timers[WEATHER] = WEATHER_TIME;
        weather = CLEAR;
      }
    }
    if (event_flags[STEALING_TIMER]) {
      timers[STEALING_TIMER] = decrement_timer(timers[STEALING_TIMER]);
      if (timers[STEALING_TIMER] == 0.0) {
        timers[STEALING_TIMER] = calc_stealing_interval();
        steal_item(&home_box);
      }
    }
  } else if (mode == COMBAT) {
    C_UNIT *cur_unit = NULL;
    for (int i = 0; i < num_npc_units; i++) {
      cur_unit = npc_units + i;
      if (cur_unit->death_animation > 0.0) {
        cur_unit->death_animation = decrement_timer(cur_unit->death_animation);
      }
    }
  }
}

float decrement_timer(float timer) {
  timer -= delta_time;
  if (timer < 0.0) {
    timer = 0.0;
  }
  return timer;
}

/*
  Generalized function to set a simple
  prompt to the screen that disappears
  in 1.5 seconds
*/
void set_prompt(const char *buffer) {
  timers[GENERAL_PROMPT] = 1.5;
  event_flags[GENERAL_PROMPT] = 1;
  vec2 prompt_pos = { 0.0, 0.25 };
  init_menu(
      prompt_pos, // position
      NULL, // on_click
      NULL, // on_hover
      NULL, // on_click_args
      NULL, // on_hover_args
      (char *) buffer, // text
      1, // enabled
      1, // textured
      0, // texture
      0.05, // text_padding
      1.0, // text_scale
      0.0, // width
      0.0, // height
      PIVOT_TOP, // pivot
      T_CENTER, // text_anchor
      get_ui_component_by_ID(GENERAL_PROMPT_ON_SCREEN) // dest
  );
}

void close_prompt() {
  get_ui_component_by_ID(GENERAL_PROMPT_ON_SCREEN)->enabled = 0;
}
