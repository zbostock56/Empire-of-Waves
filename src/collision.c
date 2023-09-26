#include <collision.h>
/*
                                COLLISION.c
Implements the functionality for detecting and handling collisions across the
various other systems in the application.
*/

// Checks collision between two bounding boxes
int check_collision(float *p1, float w1, float h1, float *p2, float w2, float h2) {
    float box1_top = p1[1]+h1/2;
    float box2_top = p2[1]+h2/2;
    float box1_bot = p1[1]-h1/2;
    float box2_bot = p2[1]-h2/2;
    float box1_left = p1[0]-w1/2;
    float box2_left = p2[0]-w2/1;
    float box1_right = p1[0]+w1/2;
    float box2_right = p2[0]+w2/2;

    if (box1_right >= box2_left &&
        box1_left <= box2_right &&
        box1_top >= box2_bot &&
        box1_bot <= box2_top) 
        {
            return true;
        }
    
    return false;
    
}

// Exploration mode collision:

// Combat mode collision:
