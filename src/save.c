#include <save.h>

int init_save_menu() {
  char *input_buffer = malloc(sizeof(char) * INPUT_BUFFER_SIZE);
  if (!input_buffer) {
    fprintf(stderr, "save.c: Unable to allocate save input buffer\n");
    return -1;
  }
  input_buffer[0] = '\0';

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
}

void close_save_menu() {
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 0;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 0;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 0;

  load_input_len = 0;
  get_ui_component_by_ID(SAVE_INPUT)->text[0] = '\0';
  open_prompt = INVALID_MENU;
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
}

void save_callback(void *arg) {
  save_game(game_save_name);
}

void load_callback(void *arg) {
  get_ui_component_by_ID(NEW_GAME)->enabled = 0;
  get_ui_component_by_ID(SAVE)->enabled = 0;
  get_ui_component_by_ID(LOAD)->enabled = 0;
  get_ui_component_by_ID(SAVE_CLOSE)->enabled = 1;
  get_ui_component_by_ID(SAVE_PROMPT)->enabled = 1;
  get_ui_component_by_ID(SAVE_INPUT)->enabled = 1;
  open_prompt = LOAD;
}

void close_callback(void *arg) {
  close_save_menu();
}

void free_save_menu() {
  free(get_ui_component_by_ID(SAVE_INPUT)->text);
}
