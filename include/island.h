/*
                                ISLAND.h
In tandem with chunk_str.h, this header defines functions whose role is to
procedurally generate islands and populate structs defined in other headers
so that the graphics side can render the islands in the viewport.
*/

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <math.h>
#include <glad/glad.h>
#include <chunk_str.h>
#include <items.h>
#include <globals.h>

/* DEFINES */
#define FREQ             (0.05)
#define DEPTH            (3)
#define X                (0)
#define Y                (1)
#define MAX_INT          (2147483647)
#define MAX_INT_FLOAT    (2147483647.0)
#define TRUE             (1)
#define FALSE            (0)
#define RETRIES          (3)
#define RETRIES_NEW_ITEM (10)
#define INVENTORY_FULL   (-1)
#define FATAL_ERROR      (-2)

/* ======================= INTERNALLY DEFINED FUNCTIONS ===================== */
int generate_island(ISLAND *);
int generate_rand();
double nano_time();
void generate_mask(float (*mask)[I_WIDTH]);
void populate_tiles(ISLAND *, float (*)[I_WIDTH]);
int gen_island_texture(ISLAND *);
void merchant_generate(MERCHANT *, ISLAND *);
unsigned int texture_from_buffer(unsigned char *, int, int, int);
void spawn_items(ISLAND *);
void item_rng(ITEM_TILES *);
void init_resource_buffer(ISLAND *);
void spawn_new_items();
int *find_rand_tile(ISLAND *, int);
void find_closest_resource(ISLAND *, vec2);
ITEM_IDS translate_resource_to_item(REC_IDS);
int pickup_resource();

/* ======================= EXTERNALLY DEFINED FUNCTIONS ===================== */
ISLAND *cur_island(CHUNK *, vec2);
float perlin(float, float, float, int, int);
I_SLOT *get_requested_inventory_slot_type(ITEM_IDS);
I_SLOT *get_player_first_empty_inventory_slot();
int are_inventory_slots_open();
void chunk_to_world(ivec2, vec2, vec2);
void update_inventory_ui();

/* MACROS */
/* Safe-guard for the chances if tile_location is */
/* set to less than zero */
#define get_tile_location(tile, island_width) {                 \
  tile = generate_rand() % (island_width * island_width) - 1;   \
  while (tile < 0) {                                            \
    tile = generate_rand() % (island_width * island_width) - 1; \
  }                                                             \
}

// ======================= INTERNALLY DEFINED FUNCTIONS ======================

// ======================= EXTERNALLY DEFINED FUNCTIONS ======================

void chunk_to_world(ivec2, vec2, vec2);
void world_to_chunk(vec2, ivec2, vec2);
