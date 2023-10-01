#include <collision.h>
#include <chunk_str.h>
#include <player_str.h>
#include <globals.h>

#define X (0)
#define Y (1)
#define PLAYER_WIDTH (1.0)
#define PLAYER_HEIGHT (1.0)
#define COORDS_X (50.5)
#define COORDS_Y (25.5)
/*
                                COLLISION.c
Implements the functionality for detecting and handling collisions across the
various other systems in the application.
*/

// Checks collision between two bounding boxes
int check_collision(float *p1, float w1, float h1, float *p2, float w2, float h2) {
    float box1_top = p1[Y]+h1/2;
    float box2_top = p2[Y]+h2/2;
    float box1_bot = p1[Y]-h1/2;
    float box2_bot = p2[Y]-h2/2;
    float box1_left = p1[X]-w1/2;
    float box2_left = p2[X]-w2/1;
    float box1_right = p1[X]+w1/2;
    float box2_right = p2[X]+w2/2;

    if (box1_right >= box2_left &&
        box1_left <= box2_right &&
        box1_top >= box2_bot &&
        box1_bot <= box2_top) 
        {
            return true;
        }
    
    return false;
}


/* 
    --------------
    --------------
    --------------
    --------------
    --------------
    player_chunk -> chunk[x, y]
    player_coord -> [[0, C_WIDTH], [0, C_WIDTH]]
    To get the tile, I need to check the island.
    Check boundary 
    chunk
    
*/
ISLAND * cur_island() {
    CHUNK cur_chunk;
    for (int i = 0; i < 9; i++) {
        if (e_player.chunk[X] == player_chunks[i].coords[X] && 
            e_player.chunk[Y] == player_chunks[i].coords[Y]) 
        {
            cur_chunk = player_chunks[i];
            break;
        }
    }
    
    /* Finds which island the the player is on by checking if they collide */
    for (int i = 0; i < cur_chunk.num_islands; i++) {
        vec2 mid_island = {cur_chunk.islands[i].coords[X] + C_WIDTH/2 - 1, cur_chunk.islands[i].coords[Y] + C_WIDTH/2 -1 };
        if (check_collision(e_player.coords, PLAYER_WIDTH, PLAYER_HEIGHT, mid_island, I_WIDTH, I_WIDTH)) {
            return &(cur_chunk.islands[i]);
        }
    }
    return NULL;
}

/* 
    Helpfer function for checking the tile's type the character is on
*/
int check_tile(ISLAND * cur_island) {
    int tile_x = truncf((e_player.coords)[X]); 
    int tile_y = truncf((e_player.coords)[Y]);
    
    return cur_island->tiles[tile_x * I_WIDTH + tile_y];
}


// Exploration mode collision:

void player_collisions() {
    int tile = check_tile(cur_island);
    /* If tile is obstructive, */
    
}
// Combat mode collision:
