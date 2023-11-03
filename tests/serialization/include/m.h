#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <globals.h>

typedef struct state {
  E_PLAYER e_player;
  GAME_MODE mode;
  vec2 home_island_coords;
  vec2 house_tile;
  float global_time;
  unsigned int num_trade_ships;
  TRADE_SHIP *trade_ships;
} STATE;

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

int compare_chunk(CHUNK *, CHUNK *);
void compare_state(STATE *);
int compare_player(E_PLAYER *, E_PLAYER *);
int compare_trade_ship(TRADE_SHIP *, TRADE_SHIP *);
void randomize_player();
void randomize_ts(TRADE_SHIP *);
void randomize_state();
void randomize_vec2(vec2, int);
void randomize_ivec2(ivec2, int);
void copy_state(STATE *);

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

GLFWwindow *init_gl();
void init_game(char *);
void init_chunks();
void manage_chunks();

int new_game(char *);
int save_game(char *);
int load_game(char *);
int load_chunk(ivec2, CHUNK *, char *);
void save_chunk(CHUNK *, char *);
