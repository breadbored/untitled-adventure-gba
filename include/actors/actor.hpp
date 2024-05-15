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

enum direction_t
{
    Down,
    Up,
    Left,
    Right
};

class Actor {
public:
    Actor(bn::sprite_ptr sprite) : sprite(sprite) {
        this->position = {0, 0};
        this->moving = false;
        this->movementCounter = 0;
        this->movementDelay = 0;
        this->direction = Down;
        this->fromPosition = {0, 0};
        this->toPosition = {0, 0};
        this->sprite = sprite;
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
    bool moving;
    uint8_t movementCounter;
    uint8_t movementDelay;
    direction_t direction;
    vector2f_t fromPosition;
    vector2f_t toPosition;
    bn::sprite_ptr sprite;
};

#endif /* ACTOR_H */