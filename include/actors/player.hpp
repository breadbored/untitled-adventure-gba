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
        sprite(sprite), 
        actor(sprite),
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
        direction(Down)
    {
        
    };
    void init(vector2f_t position);
    void draw();
    
    Actor actor;
    bn::sprite_ptr sprite;
    bn::sprite_animate_action<4> south_walking;
    bn::sprite_animate_action<4> north_walking;
    bn::sprite_animate_action<4> east_walking;
    bn::sprite_animate_action<4> west_walking;
    direction_t direction;
};;

#endif /* __PLAYER_H */