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
    COLLISION_NONE = 0,
    COLLISION_SOLID = 1024,
    COLLISION_BOTTOM_HALF = 1025,
    COLLISION_TOP_HALF = 1026,
    COLLISION_RIGHT_HALF = 1027,
    COLLISION_LEFT_HALF = 1028,
    COLLISION_TOP_RIGHT_HALF = 1029,
    COLLISION_BOTTOM_RIGHT_HALF = 1030,
    COLLISION_BOTTOM_LEFT_HALF = 1031,
    COLLISION_TOP_LEFT_HALF = 1032,
} collision_map_t;

#endif /* __MAP_H */