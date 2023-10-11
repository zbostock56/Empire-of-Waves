#include <stdio.h>
#include <player.h>
#include <chunk_str.h>
#include <trade_ship_str.h>
#include <cglm/vec2.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <globals.h>

/*
                                   MERCHANT.c
Implements the functionality for enabling/disabling merchant prompts/menus,
modifying merchant relationships, and conducting trade.
*/


// #23 - As a user I would like to talk to merchants

MERCHANT * get_closest_merchant(E_PLAYER e_player) {
    float closest_merchant_distance = 1e9+7;
    MERCHANT * closest_merchant = NULL;
    if (e_player.embarked == 0) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < player_chunks[i].num_islands; j++) { // For each island inside player's chunk
                if (player_chunks[i].islands[j].has_merchant) { // Check if island has merchant
                    // YES - island has merchant
                    if (abs(glm_vec2_norm(player_chunks[i].islands[j].merchant.coords)) - abs(glm_vec2_norm(e_player.coords)) < closest_merchant_distance) { // Check if the merchant is closer
                        // YES - the merchant is closer
                        closest_merchant_distance = abs(player_chunks[i].islands[j].merchant.coords - e_player.coords); // Update closest distance
                        closest_merchant = &player_chunks[i].islands[j].merchant; // Update closest merchant
                    }
                }
            }
        }
    }
    return closest_merchant;
}

LISTING * get_listing_item_by_number(MERCHANT * merchant, unsigned int item_number) {
    if (item_number > 0 || item_number < 10) {
        if (merchant) {
            if (merchant->listings) {
                return ((merchant->listings) + sizeof(LISTING) * (item_number - 1));
            }
        }
    }
    return NULL;
}
