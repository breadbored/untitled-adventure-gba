#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/actor.hpp"
#include "maps/map.h"
#include "bn_sprite_items_collision.h"
#include "bn_span.h"
#include "bn_tile.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_map_cell.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_fixed.h"

#define DEBUG_COLLISION false

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
        // this->toPosition = this->position;
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

int roundNumber(bn::fixed num) {
    // Get the integer part of the number
    int integerPart = static_cast<int>(num);
    
    // Get the fractional part of the number
    bn::fixed fractionalPart = num - integerPart;
    
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
    bn::fixed going_x = this->toPosition.x.round_integer() - this->position.x.round_integer();
    bn::fixed going_y = this->toPosition.y.round_integer() - this->position.y.round_integer();

    vector2f_t map_position = {
        ((this->position.x.round_integer() + ((map->width * 16) / 2)) / 16) - 1,
        ((this->position.y.round_integer() + ((map->height * 16) / 2)) / 16) + 1
    };
    vector2f_t map_to_position = {
        ((this->toPosition.x.round_integer() + (going_x >= 0 ? 1 : 0) + (map->width * 16 / 2)) / 16) - 1,
        ((this->toPosition.y.round_integer() + (going_y >= 0 ? 1 : 0) + (map->height * 16 / 2)) / 16) + 1
    };

    // Calculate the integer map positions for array indexing
    int map_index = (int)map_position.y.round_integer() * map->width + (int)map_position.x.round_integer();
    int map_to_index = (int)map_to_position.y.round_integer() * map->width + (int)map_to_position.x.round_integer();

    // Ensure the indices are within bounds
    if (map_to_index < 0 || map_to_index >= map->width * map->height) {
        return true; // Out of bounds, treat as collision
    }

    // Get the current and target tiles from the collision map
    collision_map_t current_tile = (collision_map_t)map->collision_map[map_index];
    collision_map_t collision_tile = (collision_map_t)map->collision_map[map_to_index];
    if (collision_tile == COLLISION_NONE || current_tile == COLLISION_NONE) {
        return false;
    }
    bn::sprite_tiles_ptr collision_tiles = bn::sprite_items::collision.tiles_item().create_tiles(collision_tile % 1025);
    bn::optional<bn::span<const bn::tile>> tiles_span = collision_tiles.tiles_ref();

    bool tile_map[16 * 16] = {0};

    // Fill in the tile map from the tiles span
    for (int i = 0; i < 4; i++) {
        bn::tile tile = tiles_span.value()[i];
        const uint32_t* tile_data = tile.data;
        for (int j = 0; j < 8; j++) {
            uint32_t row = tile_data[j];
            for (int k = 0; k < 8; k++) {
                int row_i = (i / 2) * 8 + j;
                int col_i = (i % 2) * 8 + k;
                // int col_i = ((i % 2) == 0 ? 1 : 0) * 8 + k; // Swapped the order of the columns
                int index_i = row_i * 16 + col_i;

                // Extract the 4-bit pixel value
                // int shift_amount = (7 - k) * 4;
                int shift_amount = (k) * 4;
                uint32_t pixel_value = (row >> shift_amount) & 0xF;
                tile_map[index_i] = pixel_value > 0;
            }
        }
    }

    // Check if the actor will collide with the tile
    int center_to_x = (int)this->toPosition.x.round_integer() % 16;
    int center_to_y = (int)this->toPosition.y.round_integer() % 16;
    int center_x = (int)this->position.x.round_integer() % 16;
    int center_y = (int)this->position.y.round_integer() % 16;

    if (center_to_x < 0) {
        center_to_x = 16 + center_to_x;
    }
    if (center_to_y < 0) {
        center_to_y = 16 + center_to_y;
    }
    if (center_x < 0) {
        center_x = 16 + center_x;
    }
    if (center_y < 0) {
        center_y = 16 + center_y;
    }

#if DEBUG
    #if DEBUG_COLLISION
        BN_LOG("\n");
        BN_LOG("", center_to_x, ",", center_to_y, "  -  ", center_x, ",", center_y);
    #endif
#endif

    bool collides = tile_map[center_to_y * 16 + center_to_x];
    bool collides_x = tile_map[center_y * 16 + center_to_x];
    bool collides_y = tile_map[center_to_y * 16 + center_x];

    // Adjust the toPosition if a collision is detected
    if (collides_x) {
        this->toPosition.x = this->position.x.round_integer();
    }
    if (collides_y) {
        this->toPosition.y = this->position.y.round_integer();
    }

    // Prevent movement if collision is detected in both x and y directions
    if (collides) {
        this->toPosition = this->position;
    }

    return collides;
}

// vector2_t getTile(int currentX, int currentY, int nextX, int nextY) {
//     return vector2_t {
//         ((nextX + ((map->width * 16) / 2)) / 16) - 1,
//         ((nextY + ((map->height * 16) / 2)) / 16) + 1
//     };
// }

// bool checkCollision(int playerX, int playerY, int nextX, int nextY) {
//     // Get the current and next tiles
//     vector2_t currentTile = getTile(playerX, playerY, nextX, nextY);
//     vector2_t nextTile = getTile(nextX, nextY, nextX, nextY);

//     int map_index = nextTile.y * map->width + nextTile.x;
//     collision_map_t collisionType = (collision_map_t)(map->collision_map[map_index] % 1025);

//     switch (collisionType) {
//         case collision_map_t::COLLISION_NONE:
//             return false;
//         case collision_map_t::VERTICAL:
//             return currentTile.x == nextTile.x && currentTile.y != nextTile.y;
//         case collision_map_t::HORIZONTAL:
//             return currentTile.y == nextTile.y && currentTile.x != nextTile.x;
//         case collision_map_t::DIAGONAL_TOP_LEFT:
//         case collision_map_t::DIAGONAL_TOP_RIGHT:
//         case collision_map_t::DIAGONAL_BOTTOM_LEFT:
//         case collision_map_t::DIAGONAL_BOTTOM_RIGHT:
//             return currentTile.x != nextTile.x && currentTile.y != nextTile.y;
//         case collision_map_t::THIN_VERTICAL_LEFT:
//             return nextX % 16 < 8;
//         case collision_map_t::THIN_VERTICAL_RIGHT:
//             return nextX % 16 >= 8;
//         case collision_map_t::THIN_HORIZONTAL_TOP:
//             return nextY % 16 < 8;
//         case collision_map_t::THIN_HORIZONTAL_BOTTOM:
//             return nextY % 16 >= 8;
//         case collision_map_t::ROUNDED_TOP_LEFT:
//             return nextX % 16 < 8 && nextY % 16 < 8;
//         case collision_map_t::ROUNDED_TOP_RIGHT:
//             return nextX % 16 >= 8 && nextY % 16 < 8;
//         case collision_map_t::ROUNDED_BOTTOM_LEFT:
//             return nextX % 16 < 8 && nextY % 16 >= 8;
//         case collision_map_t::ROUNDED_BOTTOM_RIGHT:
//             return nextX % 16 >= 8 && nextY % 16 >= 8;
//         default:
//             return false;
//     }
// }

// bool Actor::will_collide() {
//     bool collided = checkCollision(
//         this->position.x.round_integer(), 
//         this->position.y.round_integer(), 
//         this->toPosition.x.round_integer(), 
//         this->toPosition.y.round_integer()
//     );

//     if (collided) {
//         this->toPosition = this->position;
//     }

//     return collided;
// }