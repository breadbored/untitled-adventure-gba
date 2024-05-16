/**
 * @file actor.h
 */
#ifndef ACTOR_H
#define ACTOR_H

#include <math.h>
#include "utils.hpp"
#include "bn_sprites.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_animate_actions.h"

enum direction_t
{
    Down,
    Up,
    Left,
    Right
};

class Actor {
public:
    Actor(bn::sprite_ptr sprite) : 
        sprite(sprite), 
        south_walking(bn::create_sprite_animate_action_forever(
            sprite, 12, bn::sprite_items::player.tiles_item(), 1, 0, 1, 2
        )),
        west_walking(bn::create_sprite_animate_action_forever(
            sprite, 12, bn::sprite_items::player.tiles_item(), 4, 3, 4, 5
        )),
        east_walking(bn::create_sprite_animate_action_forever(
            sprite, 12, bn::sprite_items::player.tiles_item(), 7, 6, 7, 8
        )),
        north_walking(bn::create_sprite_animate_action_forever(
            sprite, 12, bn::sprite_items::player.tiles_item(), 10, 9, 10, 11
        )),
        direction(Down),
        position(vector2f_t { 0, 0 }),
        fromPosition(vector2f_t { 0, 0 }),
        toPosition(vector2f_t { 0, 0 }),
        moving(false)
    {
        
    };

    void init(vector2f_t position);
    int frame(uint32_t *animcounter);
    void draw(uint32_t *animcounter);
    bool will_collide(vector2_t toPosition);

    vector2_t getPosition() {
        return vector2_t { (int)this->position.x, (int)this->position.y };
    }
    vector2_t getFromPosition() {
        return vector2_t { (int)this->fromPosition.x, (int)this->fromPosition.y };
    }
    vector2_t getToPosition() {
        return vector2_t { (int)this->toPosition.x, (int)this->toPosition.y };
    }

    vector2f_t position;
    direction_t direction;
    vector2f_t fromPosition;
    vector2f_t toPosition;
    bool moving;
    bn::sprite_ptr sprite;
    bn::sprite_animate_action<4> south_walking;
    bn::sprite_animate_action<4> north_walking;
    bn::sprite_animate_action<4> east_walking;
    bn::sprite_animate_action<4> west_walking;
};

#endif /* ACTOR_H */