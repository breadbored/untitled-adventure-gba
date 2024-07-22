/**
 * @file dungeon.h
 */
#ifndef __DUNGEON_H
#define __DUNGEON_H

#include <math.h>
#include "utils.hpp"
#include "maps/map.h"
#include "bn_regular_bg_item.h"
#include "bn_regular_bg_ptr.h"
#include "bn_camera_actions.h"
#include "bn_rect_window.h"

/**
 **** Room shapes are defined in binary as a set of rules
 * 
 *** Lower Nibble (last 4 bits of a byte)
 * 
 * The doors are defined as a 4-bit binary number:
 * [X][X][X][X][left][top][right][bottom] where 1 is a door and 0 is a wall
 * 
 * Examples:
 * 
 * Room with doors on the left and right
 *  0b1010XXXX
 *  ╒═══════╕
 *  #       #
 *  ╘═══════╛
 * 
 * Room with doors on the left and top
 *  0b1100XXXX
 *  ╒═══#═══╕
 *  #       |
 *  ╘═══════╛
 * 
 * Room with doors on the left, top, and right
 *  0b1110XXXX
 *  ╒═══#═══╕
 *  #       #
 *  ╘═══════╛
 * 
 * Room with doors on all sides
 *  0b1111XXXX
 *  ╒═══#═══╕
 *  #       #
 *  ╘═══#═══╛
 * 
 * 
 *** Upper Nibble (first 4 bits of a byte)
 * 
 *  No use currently. Potentially will be used for special room types
 * 
 */

typedef struct room_t {
    uint8_t shape;
    map_t *map;
} room_t;

typedef struct dungeon_t {
    uint8_t start_room_index;
    uint8_t width; // number of rooms wide
    uint8_t height; // number of rooms tall
    room_t **rooms;
} dungeon_t;

class DungeonManager {
public:
    DungeonManager(dungeon_t dungeon) : 
        dungeon(dungeon), 
        current_room_index(dungeon.start_room_index),
        p_room_bg(dungeon.rooms[this->current_room_index]->map->bg_item.create_bg(0, 0)),
        p_camera(bn::camera_ptr::create(0, 0)),
        external_window(bn::rect_window::external()),
        internal_window(bn::rect_window::internal())
    {};

    void init();
    void draw();
    void redraw_room();

    int coords_to_room_index(int x, int y) {
        return (y * dungeon.width) + x;
    }
    vector2_t room_index_to_coords(int index) {
        return {index % dungeon.width, floor(index / dungeon.width)};
    }

    dungeon_t dungeon;
    uint8_t current_room_index = 0;

    bn::fixed transition_duration = 0.0f;
    bn::fixed transition_timer = 0.0f;

private:
    bn::regular_bg_ptr p_room_bg;
    bn::camera_ptr p_camera;
    bn::rect_window external_window;
    bn::rect_window internal_window;
};

#endif /* __DUNGEON_H */