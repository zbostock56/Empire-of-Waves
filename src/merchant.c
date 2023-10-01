#include <stdio.h>
#include <player.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <globals.h>
#include <cglm/vec2.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <dialog.h>

/*
                                   MERCHANT.c
Implements the functionality for enabling/disabling merchant prompts/menus,
modifying merchant relationships, and conducting trade.
*/


// #23 - As a user I would like to talk to merchants

void

MERCHANT get_closest_merchant(E_PLAYER e_player) {
    float closest_merchant_distance = 1e9+7;
    MERCHANT closest_merchant;
    if (e_player.embarked == 0) {
        for (int i = 0; i < player_chunks[4].num_islands; i++) { // For each island inside player's chunk
            if (player_chunks[4].islands[i].has_merchant) { // Check if island has merchant
                // YES - island has merchant
                // TODO: Check minus expression
                if (abs(glm_vec2_norm(player_chunks[4].islands[i].merchant.coords)) - abs(glm_vec2_norm(e_player.coords)) < closest_merchant_distance) { // Check if the merchant is closer
                    // YES - the merchant is closer
                    closest_merchant_distance = abs(player_chunks[4].islands[i].merchant.coords - e_player.coords); // Update closest distance
                    closest_merchant = player_chunks[4].islands[i].merchant; // Update closest merchant
                }
            }
        }
    }
    return closest_merchant;
}

int is_player_collide_merchant(MERCHANT merchant) {
    // TODO: Check function
    // if (check_collision(float *pos_merchant, float w_merchant, float h_merchant,
    //                     float *pos_player, float w_player, float h_player) != 0) {
    //     return true;
    // }
    return 1;
}

void merchant_interaction_listener(GLFWwindow *window, MERCHANT closest_merchant) {
    if (is_player_collide_merchant(closest_merchant)) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            open_dialog(new_dialog("dialog_name", "dialog_content"));
        }
    }
}
