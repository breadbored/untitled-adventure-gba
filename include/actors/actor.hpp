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
    Actor(bn::sprite_item sprite_item) : 
        direction(Down),
        position(vector2f_t { 0, 0 }),
        fromPosition(vector2f_t { 0, 0 }),
        toPosition(vector2f_t { 0, 0 }),
        moving(false),
        sprite_item(sprite_item),
        active_sprite(sprite_item.create_sprite(0, 0)),
        frame(0),
        frame_delay(0)
    {
        active_sprite.set_visible(false);
        // active_sprite.dimensions()
    };

    void init(vector2f_t position);
    void draw();
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
    bn::sprite_item sprite_item;

private:
    bn::sprite_ptr active_sprite;
    bool moving;
    int frame;
    int frame_delay;
};

#endif /* ACTOR_H */