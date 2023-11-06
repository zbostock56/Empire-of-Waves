#include <save.h>

int init_save_menu() {
  save_input_len = 0;
  input_buffer[0] = '\0';
  status_buffer[0] = '\0';

  // New game button
  vec2 new_game_position = { 0.0, 0.5 };
  init_menu(
    new_game_position, // position
    new_game_callback, NULL, NULL, NULL,
    "New Game", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    1.0, // text_scale
    0.5, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(NEW_GAME) // dest
  );


  // Save button
  vec2 save_position = { 0.0, 0.25 };
  init_menu(
    save_position, // position
    save_callback, NULL, NULL, NULL,
    "Save", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    1.0, // text_scale
    0.5, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(SAVE) // dest
  );

  // Load button
  vec2 load_position = { 0.0, 0.0 };
  init_menu(
    load_position, // position
    load_callback, NULL, NULL, NULL,
    "Load", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    1.0, // text_scale
    0.5, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(LOAD) // dest
  );

  // Close button
  vec2 close_position = { 0.0, -0.25 };
  init_menu(
    close_position, // position
    close_callback, NULL, NULL, NULL,
    "Close", // text
    0, // enabled
    1, // textured
    0, // texture
    0, // text_padding
    1.0, // text_scale
    0.5, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(SAVE_CLOSE) // dest
  );

  // Save prompt
  vec2 prompt_position = { 0.0, 0.25 };
  init_menu(
    prompt_position, // position
    NULL, NULL, NULL, NULL,
    "Enter the name of the save to load:", // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
    0.0, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(SAVE_PROMPT) // dest
  );

  // Save input
  vec2 input_position = { 0.0, 0.0 };
  init_menu(
    input_position, // position
    NULL, NULL, NULL, NULL,
    input_buffer, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
    0.0, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(SAVE_INPUT) // dest
  );

  // Status message
  init_menu(
    input_position, // position
    NULL, NULL, NULL, NULL,
    status_buffer, // text
    0, // enabled
    1, // textured
    0, // texture
    0.05, // text_padding
    1.0, // text_scale
    0.0, // width
    0.0, // height
    PIVOT_CENTER, // pivot
    T_CENTER, // text_anchor
    get_ui_component_by_ID(SAVE_STATUS) // dest
  );

  return 0;
}

void open_save_menu() {
  get_ui_component_by_ID(NEW_GAME)->enabled = 1;
  get_ui_component_by_ID(SAVE)->enabled = 1;
  get_ui_component_by_ID(LOAD)->enabled = 1;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 1;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 0;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 0;
  open_prompt = INVALID_MENU;
  save_input_enabled = 0;
  save_input_buffer = NULL;
  save_input_len = 0;
}

void close_save_menu() {
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 0;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 0;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 0;

  save_input_len = 0;
  get_ui_component_by_ID(SAVE_INPUT)->text[0] = '\0';
  open_prompt = INVALID_MENU;
  save_input_enabled = 0;
  save_input_buffer = NULL;
  save_input_len = 0;
}

void open_save_status(char *status) {
  if (strlen(status) >= STATUS_BUFFER_SIZE) {
    strncpy(status_buffer, status, STATUS_BUFFER_SIZE);
    status_buffer[STATUS_BUFFER_SIZE - 1] = '\0';
  } else {
    strncpy(status_buffer, status, strlen(status) + 1);
  }
  get_ui_component_by_ID(SAVE_STATUS)->enabled = 1;
}

void close_save_status() {
  get_ui_component_by_ID(SAVE_STATUS)->enabled = 0;
  get_ui_component_by_ID(SAVE_STATUS)->text[0] = '\0';
}

int save_menu_opened() {
  return (
    get_ui_component_by_ID(NEW_GAME)->enabled ||
    get_ui_component_by_ID(SAVE)->enabled ||
    get_ui_component_by_ID(LOAD)->enabled ||
    get_ui_component_by_ID(SAVE_CLOSE)->enabled ||
    get_ui_component_by_ID(SAVE_PROMPT)->enabled ||
    get_ui_component_by_ID(SAVE_INPUT)->enabled
  );
}

void new_game_callback(void *arg) {
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 1;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 1;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 1;
  open_prompt = NEW_GAME;
  save_input_enabled = 1;
  save_input_buffer = get_ui_component_by_ID(SAVE_INPUT)->text;
  save_input_len = 0;
}

void save_callback(void *arg) {
  int status = save_game(game_save_name);
  if (status) {
    open_save_status("Failed to save game");
  } else {
    open_save_status("Game saved");
  }
  save_status_interval = 1.0;
  close_save_menu();
}

void load_callback(void *arg) {
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 1;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 1;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 1;
  open_prompt = LOAD;
  save_input_enabled = 1;
  save_input_buffer = get_ui_component_by_ID(SAVE_INPUT)->text;
  save_input_len = 0;
}

void close_callback(void *arg) {
  close_save_menu();
}

void update_save_interval() {
  if (save_status_interval) {
    save_status_interval -= delta_time;
    if (save_status_interval < 0.0) {
      close_save_status();
      save_status_interval = 0.0;
    }
  }
}

int save_menu_open() {
  return get_ui_component_by_ID(NEW_GAME)->enabled ||
         get_ui_component_by_ID(SAVE)->enabled ||
         get_ui_component_by_ID(LOAD)->enabled ||
         get_ui_component_by_ID(SAVE_CLOSE)->enabled ||
         get_ui_component_by_ID(SAVE_PROMPT)->enabled ||
         get_ui_component_by_ID(SAVE_INPUT)->enabled;
}
