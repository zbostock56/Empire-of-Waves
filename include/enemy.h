#include <player_str.h>
#include <chunk_str.h>
#include <vector.h>
#include <stdio.h>

/* GLOBALS  */
extern CHUNK player_chunks[CHUNKS_SIMULATED];
extern E_PLAYER e_player;

/* DEFINES */
#define CURRENT_CHUNK (4)

typedef struct Node {
    int parent_row;
    int parent_col;
    int row;
    int col;
    int f_cost;
    int g_cost;
    int h_cost;
} Node;

/* =================== INTERNALLY DEFINED FUNCTIONS =================== */
void spawn_enemy();
int find_avail_chunks();
void generate_chunk_tiles(int [C_WIDTH][C_WIDTH], CHUNK);
void update_enemy_position(E_ENEMY *);
void pathfind_enemy(E_ENEMY *);
int search(int,int, int, int, E_ENEMY*, vector *);
void track_path(Node[C_WIDTH][C_WIDTH], vector*, int, int, int, int);
void get_cost(Node *, int, int, int, int, int, int);
void open_node(Node *, Node *, int[C_WIDTH][C_WIDTH], int[C_WIDTH][C_WIDTH], int[C_WIDTH][C_WIDTH], vector*);
/* =================== EXTERNALLY DEFINED FUNCTIONS =================== */

int double_buffer(void **, unsigned int *, unsigned int);
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);