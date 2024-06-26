/**
 * @file map.h
 */
#ifndef __MAP_H
#define __MAP_H

#include <math.h>
#include "utils.hpp"
#include "bn_sprites.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_items_title.h"

typedef enum map_items_t {
    NONE = -1,
    GRASS = 0,
    GRASS_DARK = 1,
    GRASS_LIGHT = 2,
    GRASS_DRY = 3,
    GRASS_WHEAT = 4,
    STONE = 5,
    SAND = 6,
    GRAVEL = 7,

    TREE_LIGHT_TL = 8,
    TREE_LIGHT_TR = 9,
    TREE_LIGHT_BL = 16,
    TREE_LIGHT_BR = 17,

    TREE_DARK_TL = 10,
    TREE_DARK_TR = 11,
    TREE_DARK_BL = 18,
    TREE_DARK_BR = 19,

    TREE_3_TL = 12,
    TREE_3_TR = 13,
    TREE_3_BL = 20,
    TREE_3_BR = 21,
} map_items_t;

typedef struct map_t {
    bn::regular_bg_item bg_item;
    bn::regular_bg_item bg_collision_item;
    int width;
    int height;
    const uint16_t *bg_map;
    const uint16_t *fg0_map;
    const uint16_t *fg1_map;
    const uint16_t *collision_map;
    const uint16_t *spawn_map;
    npc_t **npcs;
    uint8_t npcs_count;
} map_t;

typedef struct map_tile_t {
    int tile;
    vector2_t position;
} map_tile_t;

typedef enum collision_map_t {
    COLLISION_NONE = 0,             // No collision
    FILLED = 1,                     // Filled collision box
    THIN_HORIZONTAL_BOTTOM = 2,     // Thin horizontal collision on the bottom side
    THIN_HORIZONTAL_TOP = 3,        // Thin horizontal collision on the top side
    THIN_VERTICAL_RIGHT = 4,        // Thin vertical collision on the right side
    THIN_VERTICAL_LEFT = 5,         // Thin vertical collision on the left side
    THIN_PADDED_DIAGONAL_TOP_RIGHT = 6,         // Diagonal collision top-right to bottom-left
    THIN_PADDED_DIAGONAL_BOTTOM_RIGHT = 7,      // Diagonal collision bottom-right to top-left
    THIN_PADDED_DIAGONAL_BOTTOM_LEFT = 8,       // Diagonal collision bottom-left to top-right
    THIN_PADDED_DIAGONAL_TOP_LEFT = 9,          // Diagonal collision top-left to bottom-right
    
    CENTERED_SQUARE_SEMILARGE = 10, // Centered square collision (semi-large)
    
    MID_SIZED_HORIZONTAL_BOTTOM = 11,// Mid horizontal collision on the bottom side
    MID_SIZED_HORIZONTAL_TOP = 12,  // Mid horizontal collision on the top side
    MID_SIZED_VERTICAL_RIGHT = 13,  // Mid vertical collision on the right side
    MID_SIZED_VERTICAL_LEFT = 14,   // Mid vertical collision on the left side
    MID_PADDED_DIAGONAL_TOP_RIGHT = 15,         // Diagonal collision top-right to bottom-left
    MID_PADDED_DIAGONAL_BOTTOM_RIGHT = 16,      // Diagonal collision bottom-right to top-left
    MID_PADDED_DIAGONAL_BOTTOM_LEFT = 17,       // Diagonal collision bottom-left to top-right
    MID_PADDED_DIAGONAL_TOP_LEFT = 18,          // Diagonal collision top-left to bottom-right

    CENTERED_SQUARE_MID = 19,    // Centered square collision (mid)

    LARGE_HORIZONTAL_BOTTOM = 20,   // Large horizontal collision on the bottom side
    LARGE_HORIZONTAL_TOP = 21,      // Large horizontal collision on the top side
    LARGE_VERTICAL_RIGHT = 22,      // Large vertical collision on the right side
    LARGE_VERTICAL_LEFT = 23,       // Large vertical collision on the left side
    LARGE_PADDED_DIAGONAL_TOP_RIGHT = 24,         // Diagonal collision top-right to bottom-left
    LARGE_PADDED_DIAGONAL_BOTTOM_RIGHT = 25,      // Diagonal collision bottom-right to top-left
    LARGE_PADDED_DIAGONAL_BOTTOM_LEFT = 26,       // Diagonal collision bottom-left to top-right
    LARGE_PADDED_DIAGONAL_TOP_LEFT = 27,          // Diagonal collision top-left to bottom-right

    CENTERED_SQUARE_MID_SMALL = 28,    // Centered square collision (large)

    HUGE_HORIZONTAL_BOTTOM = 29,   // Huge horizontal collision on the bottom side
    HUGE_HORIZONTAL_TOP = 30,      // Huge horizontal collision on the top side
    HUGE_VERTICAL_RIGHT = 31,      // Huge vertical collision on the right side
    HUGE_VERTICAL_LEFT = 32,       // Huge vertical collision on the left side
    HUGE_PADDED_DIAGONAL_TOP_RIGHT = 33,         // Diagonal collision top-right to bottom-left
    HUGE_PADDED_DIAGONAL_BOTTOM_RIGHT = 34,      // Diagonal collision bottom-right to top-left
    HUGE_PADDED_DIAGONAL_BOTTOM_LEFT = 35,       // Diagonal collision bottom-left to top-right
    HUGE_PADDED_DIAGONAL_TOP_LEFT = 36,          // Diagonal collision top-left to bottom-right

    // VERTICAL = 1,                   // Vertical centered collision
    // HORIZONTAL = 2,                 // Horizontal centered collision
    // THIN_PADDED_DIAGONAL_TOP_LEFT = 3,          // Diagonal collision top-left to bottom-right
    // THIN_PADDED_DIAGONAL_TOP_RIGHT = 4,         // Diagonal collision top-right to bottom-left
    // THIN_PADDED_DIAGONAL_BOTTOM_LEFT = 5,       // Diagonal collision bottom-left to top-right
    // THIN_PADDED_DIAGONAL_BOTTOM_RIGHT = 6,      // Diagonal collision bottom-right to top-left
    // THIN_VERTICAL_LEFT = 7,         // Thin vertical collision on the left side
    // THIN_VERTICAL_RIGHT = 8,        // Thin vertical collision on the right side
    // THIN_HORIZONTAL_TOP = 9,        // Thin horizontal collision on the top side
    // THIN_HORIZONTAL_BOTTOM = 10,    // Thin horizontal collision on the bottom side
    // ROUNDED_TOP_LEFT = 11,          // Rounded edge collision top-left
    // ROUNDED_TOP_RIGHT = 12,         // Rounded edge collision top-right
    // ROUNDED_BOTTOM_LEFT = 13,       // Rounded edge collision bottom-left
    // ROUNDED_BOTTOM_RIGHT = 14,      // Rounded edge collision bottom-right
} collision_map_t;

#endif /* __MAP_H */