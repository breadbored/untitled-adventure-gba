#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/actor.hpp"
#include "maps/map.h"

void Actor::init(vector2f_t position)
{
    this->position = position;
    // this->sprite = sprite;
    // sprite.create_sprite(0, 0);
    // this->active_sprite.set_bg_priority(1);
}

void Actor::draw() {
    // Do not render if the actor is outside the screen
    vector2f_t screen_coordinates = this->position; //get_screen_coordinates(this->position);
    if (screen_coordinates.x < -(screen_size.x / 2) || screen_coordinates.x > (screen_size.x / 2) || screen_coordinates.y < -(screen_size.y / 2) || screen_coordinates.y > (screen_size.y / 2)) {
        this->active_sprite.set_visible(false);
        this->moving = false;
        return;
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
    this->active_sprite.set_x(this->position.x);
    this->active_sprite.set_y(this->position.y);
}

bool Actor::will_collide(vector2_t toPosition) {
    // if (&(player->actor) != this) {
    //     // If NPC is moving, check if it will collide with the player
    //     if ((player->actor.position.x == toPosition.x && player->actor.position.y == toPosition.y) || (player->actor.toPosition.x == toPosition.x && player->actor.toPosition.y == toPosition.y)) {
    //         return true;
    //     }
    // }
    // // TODO Implement NPC collision checking
    // // // NPCs and Player need to check collisions with each other
    // // for (size_t i = 0; i < npcs_count; i++) {
    // //     npc_t *npc = npcs[i];
    // //     if ((npc->actor.position.x == toPosition.x && npc->actor.position.y == toPosition.y) || (npc->actor.toPosition.x == toPosition.x && npc->actor.toPosition.y == toPosition.y)) {
    // //         return true;
    // //     }
    // // }

    // vector2_t toTile = (vector2_t) { toPosition.x / 32, toPosition.y / 32 };
    // vector2_t fromTile = (vector2_t) { this->fromPosition.x / 32, this->fromPosition.y / 32 };
    // size_t toIndex = toTile.x + toTile.y * map->width;
    // size_t fromIndex = fromTile.x + fromTile.y * map->width;

    // // Check collisions with walls
    // if (toTile.x < 0 || toTile.y < 0 || toTile.x >= map->width || toTile.y >= map->height) {
    //     return true;
    // }
    // // Non-conditional collisions have an id of 0, interactive collisions have an id of 1
    // if (map->collision_map[toIndex] == 0 || map->collision_map[toIndex] == 1) {
    //     return true;
    // }

    // // Check collision with foreground objects (implying standing on a raised surface) and green collisions
    // // Used to keep the player from walking off a ledge
    // if (map->fg0_map[fromIndex] != NONE && map->collision_map[toIndex] == 3) {
    //     return true;
    // }

    // // Check collision with green collisions (implying standing on a lower surface) and foreground objects (a raised surface)
    // // Used to keep the player from walking up a ledge
    // if (map->fg0_map[toIndex] != NONE && map->collision_map[fromIndex] == 3) {
    //     return true;
    // }
    
    return false;
}