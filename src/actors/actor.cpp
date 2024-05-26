#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/actor.hpp"
#include "maps/map.h"
#include "bn_sprite_items_collision.h"
#include "bn_span.h"
#include "bn_tile.h"

#define DEBUG_COLLISION true

#if DEBUG
    #if DEBUG_COLLISION
        #include "bn_log.h"
    #endif
#endif

void Actor::init(vector2f_t position)
{
    this->position = position;
    // this->sprite = sprite;
    // sprite.create_sprite(0, 0);
    // this->active_sprite.set_bg_priority(1);
}

void Actor::draw() {
    // If not a player...
    if (!this->is_player) {
        // Do not render if the actor is outside the screen
        vector2f_t screen_coordinates = {
            (this->position.x + (map->width * 16 / 2)),
            (this->position.y + (map->height * 16 / 2))
        };
        if (screen_coordinates.x < -(screen_size.x / 2) || screen_coordinates.x > (screen_size.x / 2) || screen_coordinates.y < -(screen_size.y / 2) || screen_coordinates.y > (screen_size.y / 2)) {
            this->active_sprite.set_visible(false);
            this->moving = false;
            return;
        }
    }

    // If we're drawing the sprite, we should make it visible
    if (!this->active_sprite.visible()) {
        this->active_sprite.set_visible(true);
    }

    // Get the direction the actor is moving
    // We do this because Player and Npc control their movement independently
    // and the base actor needs to be agnostic to the type of actor
    bool moving_up = this->toPosition.y - this->position.y < 0;
    bool moving_down = this->toPosition.y - this->position.y > 0;
    bool moving_left = this->toPosition.x - this->position.x < 0;
    bool moving_right = this->toPosition.x - this->position.x > 0;
    this->moving = moving_up || moving_down || moving_left || moving_right;

    if (moving_left) {
        this->direction = Left;
    } else if (moving_right) {
        this->direction = Right;
    } else if (moving_up) {
        this->direction = Up;
    } else if (moving_down) {
        this->direction = Down;
    }

    
    if (this->will_collide()) {
        this->toPosition = this->position;
    }

    // Animation
    int animation_frame = (this->frame % 4) - 1;
    if (animation_frame == 2) animation_frame = 0;
    if (this->moving) {
        if (this->frame_delay++ > 6) {
            this->frame++;
            this->frame_delay = 0;
        }
    } else {
        this->frame = 0;
        animation_frame = 0;
    }
    // Since we can only draw one direction at a time, we combine the statements
    if (this->direction == Down) {
        this->active_sprite.set_tiles(this->sprite_item.tiles_item().create_tiles(1 + animation_frame));
    } else if (this->direction == Up) {
        this->active_sprite.set_tiles(this->sprite_item.tiles_item().create_tiles(10 + animation_frame));
    } else if (this->direction == Right) {
        this->active_sprite.set_tiles(this->sprite_item.tiles_item().create_tiles(7 + animation_frame));
    } else if (this->direction == Left) {
        this->active_sprite.set_tiles(this->sprite_item.tiles_item().create_tiles(4 + animation_frame));
    }

    this->position = this->toPosition;

    if (this->is_player) {
        this->active_sprite.set_x(0);
        this->active_sprite.set_y(0);
    } else {
        this->active_sprite.set_x(this->position.x);
        this->active_sprite.set_y(this->position.y);
    }
}

int roundNumber(double num) {
    // Get the integer part of the number
    int integerPart = static_cast<int>(num);
    
    // Get the fractional part of the number
    float fractionalPart = num - integerPart;
    
    // Check if the fractional part is 0.5 or more
    if (fractionalPart >= 0.5) {
        // If positive number, round up
        if (num > 0) {
            return integerPart + 1;
        } else { // If negative number, round up (towards zero)
            return integerPart - 1;
        }
    } else {
        // If fractional part is less than 0.5, round down
        return integerPart;
    }
}

bool Actor::will_collide() {
    vector2f_t map_position = {
        (this->position.x + (map->width * 16 / 2)) / 16,
        (this->position.y + (map->height * 16 / 2)) / 16
    };
    vector2f_t map_to_position = {
        (this->toPosition.x + (map->width * 16 / 2)) / 16,
        (this->toPosition.y + (map->height * 16 / 2)) / 16
    };

    // Calculate the integer map positions for array indexing
    int map_index = (int)map_position.y * map->width + (int)map_position.x;
    int map_to_index = (int)map_to_position.y * map->width + (int)map_to_position.x;

    // Get the current and target tiles from the collision map
    collision_map_t current_tile = (collision_map_t)map->collision_map[map_index];
    collision_map_t collision_tile = (collision_map_t)map->collision_map[map_to_index];
    if (collision_tile == COLLISION_NONE) {
        return false;
    }
    bn::sprite_tiles_ptr collision_tiles = bn::sprite_items::collision.tiles_item().create_tiles(collision_tile % 1024);
    bn::optional<bn::span<const bn::tile>> tiles_span = collision_tiles.tiles_ref();
    bool tile_map[16 * 16] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    };

    // Fill in the tile map from the tiles span.
    // The tiles are 8x8, where each row is a uint32_t of 4 bits that make up a pixel
    // Each span should be 2 rows and 2 columns of bn:tile
    for (int i = 0; i < 4; i++) {
        bn::tile tile = tiles_span.value()[i];
        uint32_t *tile_data = tile.data;
        for (int j = 0; j < 8; j++) {
            uint32_t row = tile_data[j];
            for (int k = 0; k < 8; k++) {
                int row_i = (i / 2) * 8 + j;
                int col_i = (i % 2) * 8 + k;
                int index_i = row_i * 16 + col_i;
                tile_map[index_i] = (bool)((row >> (k * 4)) & 0xF);
            }
        }
    }

    // Check if the actor will collide with the tile
    int center_x = (int)this->toPosition.x % 16;
    int center_y = (int)this->toPosition.y % 16;
    return tile_map[center_y * 16 + center_x];
    
    return false;
}

// #if DEBUG
//     #if DEBUG_COLLISION
//         // Log each of the 4 corners of the full 16x16 tile
//         BN_LOG("\n");
//         BN_LOG("", tile_map[0], tile_map[15]);
//         BN_LOG("", tile_map[16 * 15], tile_map[16 * 16 - 1]);
//     #endif
// #endif