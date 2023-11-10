#include <stdio.h>
#include <string.h>
#include <player_str.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>

/* DEFINES */
#define NODE_BUF_START_LEN (4)
#define CURRENT_CHUNK (4)
#define SHIP_COLLISION_RADIUS (1.0)
#define SHIP_CHASE_RADIUS (20)
#define X (0)
#define Y (1)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

typedef enum move_mode {
  STOP,
  FORWARD,
  BACK
} MOVE_MODE;

typedef struct Node {
    int parent_row;
    int parent_col;
    int row;
    int col;
    int open;
    int checked;
    int valid_tile;
    float f_cost;
    float g_cost;
    float h_cost;
} Node;

/* =================== INTERNALLY DEFINED FUNCTIONS =================== */
void spawn_enemy();
int find_avail_chunks();
void generate_chunk_tiles(int [C_WIDTH][C_WIDTH], CHUNK);
void update_enemy_position(E_ENEMY *);
void pathfind_enemy(E_ENEMY *, unsigned int);
int search(vec2, int, int, int, int, E_ENEMY *, unsigned int, Node **,
           unsigned int *, unsigned int *);
void track_path(int arr_size_x, int arr_size_y,
                Node nodes[arr_size_x][arr_size_y],
                Node **path_list, unsigned int *path_list_len,
                unsigned int *path_list_size, int start_col, int start_row,
                int goal_col, int goal_row);
void get_cost(Node *, ivec2, ivec2, ivec2);
void open_node(Node *, Node *, Node **, unsigned int *, unsigned int *);
int add_node(Node **, unsigned int *, unsigned int *, Node *);
void delete_node(Node *, unsigned int *, unsigned int);
void update_enemy_chunk(E_ENEMY*, CHUNK*, int);
void pathfind_to_chunk(E_ENEMY *, ivec2);
void pathfind_to_shore(E_ENEMY *, CHUNK *, CHUNK *, unsigned int);

/* =================== EXTERNALLY DEFINED FUNCTIONS =================== */

int double_buffer(void **, unsigned int *, unsigned int);
void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
int circle_circle_collision(vec2, float, vec2, float);
int circle_aabb_collision(vec2, float, vec2, float, float, vec2);
int get_tile(unsigned int chunk, vec2 coords);
int spawn_projectile(vec2, vec2, float, UNIT_T);
int npc_melee_attack(C_UNIT *);
int npc_ranged_attack(C_UNIT *);
int trade_ship_active(unsigned int);
float dist_to_island(ivec2, vec2, ISLAND *);
void ship_steering(ivec2, vec2, vec2, CHUNK *, CHUNK *, unsigned int);
