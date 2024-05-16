/**
 * @file player.h
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>
#include "utils.hpp"
#include "actors/actor.hpp"
#include "bn_sprites.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_items_player.h"
#include "bn_sprite_item.h"
#include "bn_sprite_animate_actions.h"

struct controller_state_t {
    unsigned int data;
    bool A;
    bool B;
    bool down;
    bool left;
    bool right;
    bool up;
    bool start;
    bool select;
    bool L;
    bool R;
};

class Player {
public:
    Player(bn::sprite_ptr sprite) : 
        actor(sprite)
    {
        
    };
    void init(vector2f_t position);
    void draw();
    
    Actor actor;
};

#endif /* __PLAYER_H */