#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/actor.hpp"
#include "maps/map.h"

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
    if (this->will_collide()) {
        this->toPosition = this->position;
    }
    // If not a player...
    if (!this->center) {
        // Do not render if the actor is outside the screen
        vector2f_t screen_coordinates = this->position;
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

    if (this->center) {
        this->active_sprite.set_x(0);
        this->active_sprite.set_y(0);
    } else {
        this->active_sprite.set_x(this->position.x);
        this->active_sprite.set_y(this->position.y);
    }
}

bool Actor::will_collide() {
    // The actor position is center-aligned, but we want the top-left corner coordinates
    vector2_t actor_position = {
        this->getPosition().x + (map->width * 16 / 2),
        this->getPosition().y + (map->height * 16 / 2)
    };

    // The actors can move in pixel steps, but we need to know what 16x16 tile they are on
    vector2_t map_position = {
        actor_position.x / 16,
        actor_position.y / 16
    };

#if DEBUG_COLLISION
    // BN_LOG("Player position: ", this->getPosition().x);
    BN_LOG("Player map position X: ", map_position.x);
    BN_LOG("Player map position Y: ", map_position.y);
#endif
    
    return false;
}