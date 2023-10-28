#include <stdio.h>
#include <string.h>
#include <player_str.h>
#include <chunk_str.h>
#include <vector.h>
#include <trade_ship_str.h>
#include <globals.h>

/* DEFINES */
#define CURRENT_CHUNK (4)
#define SHIP_COLLISION_RADIUS (1.0)
#define SHIP_CHASE_RADIUS (20)
#define X (0)
#define Y (1)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef struct Node {
    int parent_row;
    int parent_col;
    int row;
    int col;
    float f_cost;
    float g_cost;
    float h_cost;
} Node;

/* =================== INTERNALLY DEFINED FUNCTIONS =================== */
void spawn_enemy();
int find_avail_chunks();
void generate_chunk_tiles(int [C_WIDTH][C_WIDTH], CHUNK);
void update_enemy_position(E_ENEMY *);
void pathfind_enemy(E_ENEMY *, CHUNK *);
int search(vec2, int,int, int, int, E_ENEMY*, vector *, CHUNK *);
void track_path(int arr_size_x, int arr_size_y, Node[arr_size_x][arr_size_y], vector*, int, int, int, int);
void get_cost(Node *, int, int, float, float, int, int);
void open_node(Node *, Node *, int arr_size_x, int arr_size_y, int[arr_size_x][arr_size_y], int[arr_size_x][arr_size_y], int[arr_size_x][arr_size_y], vector*);
void update_enemy_chunk(E_ENEMY*, CHUNK*, int);
/* =================== EXTERNALLY DEFINED FUNCTIONS =================== */

int double_buffer(void **, unsigned int *, unsigned int);
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
int circle_circle_collision(vec2, float, vec2, float);