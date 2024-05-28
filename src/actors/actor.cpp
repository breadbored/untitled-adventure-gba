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

#define DEBUG_COLLISION true

#if DEBUG && DEBUG_COLLISION
#include "bn_log.h"
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

vector2_t getTile(int nextX, int nextY) {
    // This is correct, because the map is centered at 0,0 and there is an offset
    return vector2_t {
        ((nextX + ((map->width * 16) / 2)) / 16) - 1,
        ((nextY + ((map->height * 16) / 2)) / 16) + 1
    };
}

int checkPlayerCollision(vector2_t newPosition) {
    // Get the tile position
    vector2_t tilePosition = getTile(newPosition.x, newPosition.y);
    
    // Calculate the inner tile coordinates (0-15 range)
    int interTileCoordinateX = (newPosition.x % 16 + 16) % 16;
    int interTileCoordinateY = (newPosition.y % 16 + 16) % 16;
    
    // Get the 1D array index for collision map
    int collisionIndex = tilePosition.y * map->width + tilePosition.x;
    
    // Retrieve the collision_map_t value and mod it by 1025
    return (int)(map->collision_map[collisionIndex] % 1025);
}

// Shitty collision system
bool Actor::will_collide() {
    vector2f_t directionExact = vector2f_t {
        this->toPosition.x - this->position.x,
        this->toPosition.y - this->position.y
    };
    vector2_t direction = vector2_t {
        directionExact.x.round_integer() == 0 ? 0 : directionExact.x.round_integer() / abs(directionExact.x.round_integer()),
        directionExact.y.round_integer() == 0 ? 0 : directionExact.y.round_integer() / abs(directionExact.y.round_integer())
    };

    int currentTileLayer = checkPlayerCollision(vector2_t {this->position.x.round_integer(), this->position.y.round_integer()});
    int nextTileLayer = checkPlayerCollision(vector2_t {this->toPosition.x.round_integer(), this->toPosition.y.round_integer()});

    if (abs(currentTileLayer - nextTileLayer) > 1) {
        this->toPosition = this->position;
        return true;
    }

    return false;
}