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

class Player {
public:
    Player(bn::sprite_item sprite_item) : 
        actor(sprite_item, true)
    {};
    void init(vector2f_t position);
    void draw();
    
    Actor actor;
};

#endif /* __PLAYER_H */