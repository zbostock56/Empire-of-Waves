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
  if (rand() % 100 == 0 && weather == CLEAR) {
    weather = FOG;
    event_flags[WEATHER_TIMER] = ENABLED;
    timers[WEATHER_TIMER] = WEATHER_TIME;
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
    if (event_flags[HEALTH_REDUCTION_TIMER]) {
      timers[HEALTH_REDUCTION_TIMER] -= delta_time;
      if (timers[HEALTH_REDUCTION_TIMER] <= 0.0) {
        decrease_health();
        reset_health_reduction_timer();
      }
    }

    if (event_flags[HEALTH_INCREASE_TIMER]) {
      timers[HEALTH_INCREASE_TIMER] -= delta_time;
      if (timers[HEALTH_INCREASE_TIMER] <= 0.0) {
        increase_health();
        reset_health_increase_timer();
      }
    }
  }

  /*
    Hunger statistic timer control
  */
  if (event_flags[HUNGER_TIMER] && mode == EXPLORATION) {
    timers[HUNGER_TIMER] -= delta_time;
    if (e_player.hunger > MIN_HUNGER_BEFORE_HEALTH_REDUCTION &&
        event_flags[HEALTH_REDUCTION_TIMER]) {
      /* Hunger has been restored to high enough level, stop */
      /* removing health from the player */
      stop_health_reduction_timer();
    } else if (e_player.hunger > MAX_HUNGER_BEFORE_HEALTH_INCREASE &&
               !event_flags[HEALTH_INCREASE_TIMER]) {
      /* Player has now gotten into the health increase range, */
      /* start health increasing */
      start_health_increase_timer();
    } else if (e_player.hunger <= MAX_HUNGER_BEFORE_HEALTH_INCREASE &&
               event_flags[HEALTH_INCREASE_TIMER]) {
      /* Player is now outside the health increase range,  */
      /* stop health increasing */
      stop_health_increase_timer();
    }
    if (timers[HUNGER_TIMER] <= 0.0) {
      decrease_hunger();
      if (e_player.hunger <= MIN_HUNGER_BEFORE_HEALTH_REDUCTION &&
          !event_flags[HEALTH_REDUCTION_TIMER]) {
        /* Hunger has reached critical low, begin decrementing */
        /* health from the player */
        start_health_reduction_timer();
      }
      reset_hunger_timer();
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

    if (event_flags[WEATHER_TIMER]) {
      timers[WEATHER_TIMER] = decrement_timer(timers[WEATHER_TIMER]);
      if (timers[WEATHER_TIMER] == 0.0) {
        event_flags[WEATHER_TIMER] = DISABLED;
        timers[WEATHER_TIMER] = WEATHER_TIME;
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

/* =============== HUNGER HELPERS ==================== */
/*
  Helper function to start the hunger reduction timer

  Hunger drops by 1.0 every 10 seconds
*/
void start_hunger_timer() {
  reset_hunger_timer();
  event_flags[HUNGER_TIMER] = ENABLED;
}

void stop_health_reduction_timer() {
  event_flags[HEALTH_REDUCTION_TIMER] = DISABLED;
}

void stop_health_increase_timer() {
  event_flags[HEALTH_INCREASE_TIMER] = DISABLED;
}

void start_health_reduction_timer() {
  reset_health_reduction_timer();
  event_flags[HEALTH_REDUCTION_TIMER] = ENABLED;
}

void start_health_increase_timer() {
  reset_health_increase_timer();
  event_flags[HEALTH_INCREASE_TIMER] = ENABLED;
}

void reset_health_reduction_timer() {
  timers[HEALTH_REDUCTION_TIMER] = HEALTH_REDUCTION_TIME;
}

void reset_health_increase_timer() {
  timers[HEALTH_INCREASE_TIMER] = HEALTH_INCREASE_TIME;
}

void decrease_hunger() {
  e_player.hunger -= 1.0;
  if (e_player.hunger <= 0.0) {
    e_player.hunger = 0.0;
  }
}

void decrease_health() {
  c_player.health -= 3.0;
  if (c_player.health <= 0.0) {
    respawn_player();
  }
}

void increase_health() {
  c_player.health += 5.0;
  if (c_player.health >= c_player.max_health) {
    c_player.health = c_player.max_health;
  }
}

void reset_hunger_timer() {
  timers[HUNGER_TIMER] = hunger_timer;
}

/*
  Adjusts the hunger timer for the console
*/
void set_hunger_timer(float new_time) {
  hunger_timer = new_time;
  reset_hunger_timer();
}

/* ================================================= */

/*
  Generalized function to set a simple
  prompt to the screen that disappears
  in 1.5 seconds
*/
void set_prompt(const char *buffer) {
  timers[GENERAL_PROMPT] = GENERAL_PROMPT_TIME;
  event_flags[GENERAL_PROMPT] = ENABLED;
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
