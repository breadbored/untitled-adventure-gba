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
#include "bn_sprite_items_sword_swipe.h"

#if DEBUG && DEBUG_FACING_DOT
#include "bn_sprite_items_debug_dot.h"
#endif

class Player {
public:
    Player(bn::sprite_item sprite_item) : 
        actor(sprite_item, true),
        sword(bn::sprite_items::sword_swipe.create_sprite(0,0))
#if DEBUG && DEBUG_FACING_DOT
        , facing_dot(bn::sprite_items::debug_dot.create_sprite(0, 0))
#endif
    {
        // this->sword
    };
    void init(vector2f_t position);
    void draw();
    
    Actor actor;

    bn::sprite_ptr sword;
#if DEBUG && DEBUG_FACING_DOT
    bn::sprite_ptr facing_dot;
#endif
};

#endif /* __PLAYER_H */