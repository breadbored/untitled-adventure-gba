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
    // this->sprite.set_bg_priority(1);
}

int Actor::frame(uint32_t *animcounter) {
    int playerFrame = 0;
    if (!this->moving) {
        if (this->direction == Down) {
            playerFrame = 1;
        } else if (this->direction == Up) {
            playerFrame = 10;
        } else if (this->direction == Left) {
            playerFrame = 4;
        } else if (this->direction == Right) {
            playerFrame = 7;
        }
    } else {
        int frameCount = ((*animcounter) / 2) % 4;
        if (this->direction == Down) {
            if (frameCount == 1 || frameCount == 3) playerFrame = 1;
            else playerFrame = 1 + (frameCount == 0 ? -1 : 1);
        } else if (this->direction == Up) {
            if (frameCount == 1 || frameCount == 3) playerFrame = 10;
            else playerFrame = 10 + (frameCount == 0 ? -1 : 1);
        } else if (this->direction == Left) {
            if (frameCount == 1 || frameCount == 3) playerFrame = 4;
            else playerFrame = 4 + (frameCount == 0 ? -1 : 1);
        } else if (this->direction == Right) {
            if (frameCount == 1 || frameCount == 3) playerFrame = 7;
            else playerFrame = 7 + (frameCount == 0 ? -1 : 1);
        }
    }
    return playerFrame;
}

void Actor::draw(uint32_t *animcounter) {
    vector2_t screen_coordinates = get_screen_coordinates(this->getPosition());

    // Do not render if the actor is outside the screen
    if (screen_coordinates.x < -64 || screen_coordinates.x > screen_size.x + 64 || screen_coordinates.y < -64 || screen_coordinates.y > screen_size.y + 64) {
        return;
    }

    int actorFrame = Actor::frame(animcounter);

    // Old N64 RDP Drawing
    // rdp_sync( SYNC_PIPE );
    // rdp_load_texture_stride( 0, 0, MIRROR_DISABLED, this->sprite, actorFrame );
    // rdp_draw_sprite( 0, screen_coordinates.x, screen_coordinates.y, this->direction == Right ? MIRROR_X : MIRROR_DISABLED );
    
    // GBA Sprite Drawing
    // this->sprite
    // this->sprite.set_tile_index(actorFrame);
}

bool Actor::will_collide(vector2_t toPosition) {
    if (&(player->actor) != this) {
        // If NPC is moving, check if it will collide with the player
        if ((player->actor.position.x == toPosition.x && player->actor.position.y == toPosition.y) || (player->actor.toPosition.x == toPosition.x && player->actor.toPosition.y == toPosition.y)) {
            return true;
        }
    }
    // TODO Implement NPC collision checking
    // // NPCs and Player need to check collisions with each other
    // for (size_t i = 0; i < npcs_count; i++) {
    //     npc_t *npc = npcs[i];
    //     if ((npc->actor.position.x == toPosition.x && npc->actor.position.y == toPosition.y) || (npc->actor.toPosition.x == toPosition.x && npc->actor.toPosition.y == toPosition.y)) {
    //         return true;
    //     }
    // }

    vector2_t toTile = (vector2_t) { toPosition.x / 32, toPosition.y / 32 };
    vector2_t fromTile = (vector2_t) { this->fromPosition.x / 32, this->fromPosition.y / 32 };
    size_t toIndex = toTile.x + toTile.y * map->width;
    size_t fromIndex = fromTile.x + fromTile.y * map->width;

    // Check collisions with walls
    if (toTile.x < 0 || toTile.y < 0 || toTile.x >= map->width || toTile.y >= map->height) {
        return true;
    }
    // Non-conditional collisions have an id of 0, interactive collisions have an id of 1
    if (map->collision_map[toIndex] == 0 || map->collision_map[toIndex] == 1) {
        return true;
    }

    // Check collision with foreground objects (implying standing on a raised surface) and green collisions
    // Used to keep the player from walking off a ledge
    if (map->fg0_map[fromIndex] != NONE && map->collision_map[toIndex] == 3) {
        return true;
    }

    // Check collision with green collisions (implying standing on a lower surface) and foreground objects (a raised surface)
    // Used to keep the player from walking up a ledge
    if (map->fg0_map[toIndex] != NONE && map->collision_map[fromIndex] == 3) {
        return true;
    }
    
    return false;
}