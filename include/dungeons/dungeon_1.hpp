/**
 * @file dungeon.h
 */
#ifndef __DUNGEON_1_H
#define __DUNGEON_1_H

#include <math.h>
#include "utils.hpp"
#include "maps/map.h"
#include "dungeons/dungeon.hpp"

// Generated includes for the dungeon rooms
#include "maps/dungeon_all_directions_map.h"

// Individual rooms are defined here to clean up the format of the `rooms` array
room_t dungeon_1_room_1 = {
    .shape = 0b0000'1111,
    .map = &dungeon_all_directions_map
};
room_t dungeon_1_room_2 = {
    .shape = 0b0000'1111,
    .map = &dungeon_all_directions_map
};

// The dungeon layout
uint8_t dungeon_1_start_room_index = 0;
uint8_t dungeon_1_width = 2;
uint8_t dungeon_1_height = 2;
room_t* dungeon_1_rooms[] = {
    &dungeon_1_room_1, &dungeon_1_room_2,
    &dungeon_1_room_1, &dungeon_1_room_2,
};

// The dungeon itself
dungeon_t dungeon_1 = {
    .start_room_index = dungeon_1_start_room_index,
    .width = dungeon_1_width,
    .height = dungeon_1_height,
    .rooms = dungeon_1_rooms
};

#endif /* __DUNGEON_1_H */