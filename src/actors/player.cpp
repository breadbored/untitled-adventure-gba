#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/player.hpp"
#include "actors/actor.hpp"
#include "bn_sprite_items_player.h"

void Player::draw()
{
    /**
     * Controller inputs per draw frame
    */

    if (!this->actor.animate) {
        // Since multiple directions can be held, we separate the statements
        vector2f_t move = { 0.0, 0.0 };
        if (bn::keypad::left_held() || bn::keypad::left_pressed()) {
            move.x -= 0.75;
        }
        if (bn::keypad::right_held() || bn::keypad::right_pressed()) {
            move.x += 0.75;
        } 
        if (bn::keypad::up_held() || bn::keypad::up_pressed()) {
            move.y -= 0.75;
        } 
        if (bn::keypad::down_held() || bn::keypad::down_pressed()) {
            move.y += 0.75;
        }
        vector2f_t normalized = normalize(move, 0.75);
        this->actor.toPosition.x = this->actor.position.x + normalized.x;
        this->actor.toPosition.y = this->actor.position.y + normalized.y;
    }
    
    this->actor.draw();
}
