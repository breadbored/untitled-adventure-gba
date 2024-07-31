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
#include "utils.hpp"
#include "bn_sprite_items_sword_swipe.h"

#if DEBUG
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

    vector2f_t toPosition = this->toPosition;

    if (this->animate) {
        if (toPosition.x == this->position.x && toPosition.y == this->position.y) {
            this->animate = false;
        } else {
            this->position = this->fromPosition;
            vector2f_t move = { 
                toPosition.x - this->position.x, 
                toPosition.y - this->position.y 
            };
            bn::fixed distance = Qsqrt(move.x * move.x + move.y * move.y);
            if (distance > 0.75f) {
                move.x *= 0.75f / distance;
                move.y *= 0.75f / distance;
            }
            toPosition.x = this->position.x + move.x;
            toPosition.y = this->position.y + move.y;
        }
    }

    // If we're drawing the sprite, we should make it visible
    if (!this->active_sprite.visible()) {
        this->active_sprite.set_visible(true);
    }

    // Get the direction the actor is moving
    // We do this because Player and Npc control their movement independently
    // and the base actor needs to be agnostic to the type of actor
    bool moving_up = toPosition.y - this->position.y < 0;
    bool moving_down = toPosition.y - this->position.y > 0;
    bool moving_left = toPosition.x - this->position.x < 0;
    bool moving_right = toPosition.x - this->position.x > 0;
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

    // Weapon Swipe Animation Logic
    //Code to figure out where the weapon swipe animation needs to play.
    if(this->direction == Left) {
        this->weapon_swipe.set_position(player->actor.position.x - player->actor.active_sprite.dimensions().width()/2, player->actor.position.y);
        this->weapon_swipe.set_z_order(-1);
    } else if(this->direction == Right) {
        this->weapon_swipe.set_position(player->actor.position.x + player->actor.active_sprite.dimensions().width()/2, player->actor.position.y);
        this->weapon_swipe.set_z_order(-1);
    } else if(this->direction == Up) {
        this->weapon_swipe.set_position(player->actor.position.x, player->actor.position.y - player->actor.active_sprite.dimensions().height()/2);
        this->weapon_swipe.set_z_order(0);
    } else {
        this->weapon_swipe.set_position(player->actor.position.x, player->actor.position.y + player->actor.active_sprite.dimensions().height()/2);
        this->weapon_swipe.set_z_order(-1);
    }
    if (!this->weapon_swipe.camera().has_value()) {
        this->weapon_swipe.set_camera(this->active_sprite.camera());
    }
    
    //Code to turn the animation on for a set amount of frames and turn it back off.
    if(this->swiping == true && this->swiping_timer > 0) {
        this->swiping_timer--;
        this->weapon_swipe.set_visible(true);
        this->animate = true;
    }
    else {
        this->weapon_swipe.set_visible(false);
        this->swiping = false;
        // don't kill the animate in the `else` case, it stops screen transitions
        // this->animate = false;
    }

    this->check_collision();
    if (!this->animate) {
        toPosition = this->toPosition;
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

    this->position = toPosition;
    this->fromPosition = toPosition;

    this->active_sprite.set_x(this->position.x);
    this->active_sprite.set_y(this->position.y);
}

vector2_t getTile(int nextX, int nextY) {
    // This is correct, because the map is centered at 0,0 and there is an offset
    return vector2_t {
        ((nextX + ((map->width * 16) / 2)) / 16),
        ((nextY + ((map->height * 16) / 2)) / 16)
    };
}

int checkPlayerCollision(vector2_t newPosition) {
    // Get the tile position
    vector2_t tilePosition = getTile(newPosition.x, newPosition.y);

    // Get the 1D array index for collision map
    int collisionIndex = tilePosition.y * map->width + tilePosition.x;
    
    // Retrieve the collision_map_t value and mod it by 1025
    return (int)(map->collision_map[collisionIndex] % 1024);
}

bool check_colliding_within_tile(int tile, vector2_t newPosition) {
    // Calculate the inner tile coordinates (0-15 range)
    int interTileCoordinateX = (newPosition.x % 16 + 16) % 16;
    int interTileCoordinateY = (newPosition.y % 16 + 16) % 16;
    
    #if DEBUG && DEBUG_COLLISION
        BN_LOG("Tile: ", tile, "; tileCoordX", interTileCoordinateX, "; tileCoordY", interTileCoordinateY);
    #endif

    switch (tile) {
        case COLLISION_NONE:
            return false;
        case FILLED:
            return true;
        
        // THIN COLLISIONS (2px wide walls)
        case THIN_HORIZONTAL_BOTTOM:
            return interTileCoordinateY > 14;
        case THIN_HORIZONTAL_TOP:
            return interTileCoordinateY < 2;
        case THIN_VERTICAL_RIGHT:
            return interTileCoordinateX > 14;
        case THIN_VERTICAL_LEFT:
            return interTileCoordinateX < 2;
        case THIN_PADDED_DIAGONAL_TOP_LEFT:
            return interTileCoordinateY <= -interTileCoordinateX + 16 + 1;
        case THIN_PADDED_DIAGONAL_BOTTOM_RIGHT:
            return interTileCoordinateY >= -interTileCoordinateX + 16 - 1;
        case THIN_PADDED_DIAGONAL_BOTTOM_LEFT:
            return interTileCoordinateY >= interTileCoordinateX - 1;
        case THIN_PADDED_DIAGONAL_TOP_RIGHT:
            return interTileCoordinateY <= interTileCoordinateX + 1;
        
        // MID-SIZED COLLISIONS (4px wide walls)
        case MID_SIZED_HORIZONTAL_BOTTOM:
            return interTileCoordinateY > 12;
        case MID_SIZED_HORIZONTAL_TOP:
            return interTileCoordinateY < 4;
        case MID_SIZED_VERTICAL_RIGHT:
            return interTileCoordinateX > 12;
        case MID_SIZED_VERTICAL_LEFT:
            return interTileCoordinateX < 4;
        case MID_PADDED_DIAGONAL_TOP_LEFT:
            return interTileCoordinateY <= -interTileCoordinateX + 16 + 3;
        case MID_PADDED_DIAGONAL_BOTTOM_RIGHT:
            return interTileCoordinateY >= -interTileCoordinateX + 16 - 3;
        case MID_PADDED_DIAGONAL_BOTTOM_LEFT:
            return interTileCoordinateY >= interTileCoordinateX - 3;
        case MID_PADDED_DIAGONAL_TOP_RIGHT:
            return interTileCoordinateY <= interTileCoordinateX + 3;

        // LARGE COLLISIONS (6px wide walls)
        case LARGE_HORIZONTAL_BOTTOM:
            return interTileCoordinateY > 10;
        case LARGE_HORIZONTAL_TOP:
            return interTileCoordinateY < 6;
        case LARGE_VERTICAL_RIGHT:
            return interTileCoordinateX > 10;
        case LARGE_VERTICAL_LEFT:
            return interTileCoordinateX < 6;
        case LARGE_PADDED_DIAGONAL_TOP_LEFT:
            return interTileCoordinateY <= -interTileCoordinateX + 16 + 5;
        case LARGE_PADDED_DIAGONAL_BOTTOM_RIGHT:
            return interTileCoordinateY >= -interTileCoordinateX + 16 - 5;
        case LARGE_PADDED_DIAGONAL_BOTTOM_LEFT:
            return interTileCoordinateY >= interTileCoordinateX - 5;
        case LARGE_PADDED_DIAGONAL_TOP_RIGHT:
            return interTileCoordinateY <= interTileCoordinateX + 5;

        // HUGE COLLISIONS (8px wide walls)
        case HUGE_HORIZONTAL_BOTTOM:
            return interTileCoordinateY > 8;
        case HUGE_HORIZONTAL_TOP:
            return interTileCoordinateY < 8;
        case HUGE_VERTICAL_RIGHT:
            return interTileCoordinateX > 8;
        case HUGE_VERTICAL_LEFT:
            return interTileCoordinateX < 8;
        case HUGE_PADDED_DIAGONAL_TOP_LEFT:
            return interTileCoordinateY <= -interTileCoordinateX + 16 + 7;
        case HUGE_PADDED_DIAGONAL_BOTTOM_RIGHT:
            return interTileCoordinateY >= -interTileCoordinateX + 16 - 7;
        case HUGE_PADDED_DIAGONAL_BOTTOM_LEFT:
            return interTileCoordinateY >= interTileCoordinateX - 7;
        case HUGE_PADDED_DIAGONAL_TOP_RIGHT:
            return interTileCoordinateY <= interTileCoordinateX + 7;
        
        // CENTERED SQUARE COLLISION (8px wide walls)
        default:
            return false;
    }
}

// Shitty collision system
bool Actor::check_collision() {
    int currentTileLayer = checkPlayerCollision(vector2_t {this->position.x.round_integer(), this->position.y.round_integer() + 6});
    int nextTileLayer = checkPlayerCollision(vector2_t {this->toPosition.x.round_integer(), this->toPosition.y.round_integer() + 6});
    int xNextTileLayer = checkPlayerCollision(vector2_t {this->toPosition.x.round_integer(), this->position.y.round_integer() + 6});
    int yNextTileLayer = checkPlayerCollision(vector2_t {this->position.x.round_integer(), this->toPosition.y.round_integer() + 6});

    bool colliding_x = check_colliding_within_tile(xNextTileLayer, vector2_t {this->toPosition.x.round_integer(), this->position.y.round_integer() + 6});
    bool colliding_y = check_colliding_within_tile(yNextTileLayer, vector2_t {this->position.x.round_integer(), this->toPosition.y.round_integer() + 6});
    bool colliding = check_colliding_within_tile(nextTileLayer, vector2_t {this->toPosition.x.round_integer(), this->toPosition.y.round_integer() + 6});

    #if DEBUG && DEBUG_COLLISION
        BN_LOG("colliding: ", colliding ? "true" : "false");
    #endif

    // Check collision and adjust movement
    if (colliding) {
        // Collision in intended position, do not move
        this->toPosition = this->position;
    } else if (colliding_x) {
        // Collision in x direction, move only in y direction
        this->toPosition.x = this->position.x;
    } else if (colliding_y) {
        // Collision in y direction, move only in x direction
        this->toPosition.y = this->position.y;
    }

    return false;
}