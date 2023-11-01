#include <stdio.h>
#include <ui_component.h>
#include <menu.h>
#include <globals.h>

#define INPUT_BUFFER_SIZE (16)
#define STATUS_BUFFER_SIZE (32)

float save_status_interval = 0.0;
UI_ID open_prompt = INVALID_MENU;
char input_buffer[INPUT_BUFFER_SIZE];
char status_buffer[STATUS_BUFFER_SIZE];
int save_input_enabled = 0;
char *save_input_buffer = NULL;
unsigned int save_input_len = 0;

/* ================= INTERNALLY DEFINED FUNCTIONS ================= */
void new_game_callback(void *);
void save_callback(void *);
void load_callback(void *);
void close_callback(void *);

/* ================= EXTERNALLY DEFINED FUNCTIONS ================ */
int save_game(char *);
void init_menu(vec2, void (*)(void *), void (*)(void *), void *, void *,
               char *, int, int, unsigned int, float, float, float, float,
               PIVOT, TEXT_ANCHOR, UI_COMPONENT *);
