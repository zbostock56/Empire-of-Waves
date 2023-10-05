#include <collision.h>

/*
                                COLLISION.c
Implements the functionality for detecting and handling collisions across the
various other systems in the application.
*/

/* Checks collision between two bounding boxes */
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
   helper function for getting the island player's currently on 
*/
int cur_island() {
    CHUNK * cur_chunk = &player_chunks[4];

    /* Finds which island the the player is on by checking if they collide */
    for (int i = 0; i < cur_chunk->num_islands; i++) {
        vec2 mid_island = {cur_chunk->islands[i].coords[X] + C_WIDTH/2 - 1, cur_chunk->islands[i].coords[Y] + C_WIDTH/2 -1 };
        vec2 mid_island_world = GLM_VEC2_ZERO_INIT;
        vec2 player_world = GLM_VEC2_ZERO_INIT;
        
        /* Converting positions to global coordinates */
        chunk_to_world(e_player.chunk, mid_island, mid_island_world);
        chunk_to_world(e_player.chunk, e_player.coords, player_world);
        if (check_collision(player_world, PLAYER_WIDTH, PLAYER_HEIGHT, mid_island_world, I_WIDTH, I_WIDTH)) {
            return i;
        }
    }
    
    return -1;
}

/* 
    Helper function for checking the tile's type of a given local coordinate
*/
int check_tile(int cur_isl, vec2 coords) {
    
    /* If it is not in an island, the tile should be an ocean tile */
    if (cur_isl == -1) {
        return OCEAN;
    }
    int tile_x = truncf((coords)[X]); 
    int tile_y = truncf((coords)[Y]);
    CHUNK * cur_chunk = &player_chunks[4];
    printf("Tile X : %d Tile Y : %d\n", tile_x, tile_y);
    return cur_chunk->islands[cur_isl].tiles[tile_y * I_WIDTH + tile_x];
}

// Exploration mode collision:

/*
    Handles the collision of a character
*/
int player_collisions(vec2 coords) {
    // int tile = check_tile(cur_island);
    /* If tile is obstructive, */
    
    int tile = check_tile(cur_island(), coords);
    if (tile == OCEAN || tile == SHORE) {
        return true;
    }
    return false;
}

/*
    Handles the collision of a ship
*/
int ship_collisions(vec2 coords) {
    int tile = check_tile(cur_island(), coords);
    if (tile == OCEAN || tile == SHORE) {
        return false;
    }
    return true;
}

// Combat mode collision:
