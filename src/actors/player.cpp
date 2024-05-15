#include <math.h>
#include "globals.h"
#include "utils.hpp"
#include "actors/player.hpp"
#include "bn_sprite_items_player.h"

static const uint8_t movementDelayThreshold = 2;

void Player::init(vector2f_t position)
{
    bn::sprite_ptr sprite = bn::sprite_items::player.create_sprite(0, 0);
    this->actor.init(position);
}

void Player::draw()
{
    // Direction
    // Since multiple directions can be held, we separate the statements
    // if (bn::keypad::left_held() && !player->direction == Left && buttonPresses[Left] == 0) {
    //   buttonPresses[Left] = button_timer.elapsed_ticks();
    // } else if (!bn::keypad::left_held()) {
    //   buttonPresses[Left] = 0;
    // }
    // if (bn::keypad::right_held() && !player->direction == Right && buttonPresses[Right] == 0) {
    //   buttonPresses[Right] = button_timer.elapsed_ticks();
    // } else if (!bn::keypad::right_held()) {
    //   buttonPresses[Right] = 0;
    // }
    // if (bn::keypad::up_held() && !player->direction == Up && buttonPresses[Up] == 0) {
    //   buttonPresses[Up] = button_timer.elapsed_ticks();
    // } else if (!bn::keypad::up_held()) {
    //   buttonPresses[Up] = 0;
    // }
    // if (bn::keypad::down_held() && !player->direction == Down && buttonPresses[Down] == 0) {
    //   buttonPresses[Down] = button_timer.elapsed_ticks();
    // } else if (!bn::keypad::down_held()) {
    //   buttonPresses[Down] = 0;
    // }
    // // Find the most recent button press
    // int mostRecentPress = 0;
    // direction_t mostRecentDirection = (direction_t)0;
    // for (int i = 0; i < MAX_BUTTON_PRESSES; i++) {
    //     if (buttonPresses[i] > mostRecentPress) {
    //         mostRecentPress = buttonPresses[i];
    //         mostRecentDirection = (direction_t)i;
    //     }
    // }
    // // Set the player direction to the most recent button press
    // if (mostRecentPress != 0) {
    //     player->direction = mostRecentDirection;
    // }
    if (bn::keypad::left_held()) {
      player->direction = Left;
    } else if (bn::keypad::right_held()) {
      player->direction = Right;
    } else if (bn::keypad::up_held()) {
      player->direction = Up;
    } else if (bn::keypad::down_held()) {
      player->direction = Down;
    }

    // "Physics"
    // Since multiple directions can be held, we separate the statements
    vector2f_t move = { 0.0, 0.0 };
    if (bn::keypad::left_held()) {
      move.x -= 0.75;
    }
    if (bn::keypad::right_held()) {
      move.x += 0.75;
    } 
    if (bn::keypad::up_held()) {
      move.y -= 0.75;
    } 
    if (bn::keypad::down_held()) {
      move.y += 0.75;
    }
    vector2f_t normalized = normalize(move, 0.75);
    debug_text.clear();
    // bn::string<100> debug_text_string;
    // debug_text_string.format("Debug %s, %s", normalized.x, normalized.y);
    text_generator.generate(
      0, 
      -30, 
      bn::format<100>(
        bn::string_view("Debug {}.{}, {}.{}"), 
        (int)normalized.x, 
        abs((int)(normalized.x * 1000) % 1000),
        (int)normalized.y,
        abs((int)(normalized.y * 1000) % 1000)
        ), 
      debug_text
    );
    player->actor.position.x += normalized.x;
    player->actor.position.y += normalized.y;
    player->sprite.set_x(player->actor.getPosition().x);
    player->sprite.set_y(player->actor.getPosition().y);

    // Animation
    // Since we can only draw one direction at a time, we combine the statements
    if (bn::keypad::down_held() && normalized.y != 0) {
      player->south_walking.update();

      player->north_walking.reset();
      player->west_walking.reset();
      player->east_walking.reset();
    } else if (bn::keypad::up_held() && normalized.y != 0) {
      player->north_walking.update();

      player->south_walking.reset();
      player->west_walking.reset();
      player->east_walking.reset();
    } else if (bn::keypad::right_held() && normalized.x != 0) {
      player->east_walking.update();

      player->south_walking.reset();
      player->north_walking.reset();
      player->west_walking.reset();
    } else if (bn::keypad::left_held() && normalized.x != 0) {
      player->west_walking.update();

      player->south_walking.reset();
      player->north_walking.reset();
      player->east_walking.reset();
    }

    // Idle
    if (
      !bn::keypad::left_held() &&
      !bn::keypad::right_held() &&
      !bn::keypad::up_held() &&
      !bn::keypad::down_held()
    ) {
      if (player->direction == Down) {
        player->south_walking.reset();
        player->south_walking.update();
      } else if (player->direction == Up) {
        player->north_walking.reset();
        player->north_walking.update();
      } else if (player->direction == Left) {
        player->west_walking.reset();
        player->west_walking.update();
      } else if (player->direction == Right) {
        player->east_walking.reset();
        player->east_walking.update();
      }
    }
    
    this->actor.draw(&animcounter);
}

void Player::handle_controller(controller_state_t *controller_state) {
    // Player Movement
    if (!(controller_state->up || controller_state->down || controller_state->left || controller_state->right)) {
        this->actor.movementDelay = 0;
    }

    if (!this->actor.moving) {
        if( controller_state->down && controller_state->down >= controller_state->up && controller_state->down >= controller_state->left && controller_state->down >= controller_state->right ) {
            this->actor.direction = Down;
            vector2_t toPosition = {
                this->actor.fromPosition.x,
                floor(this->actor.fromPosition.y / 32) * 32 + 32
            };
            if (this->actor.movementDelay > movementDelayThreshold && !this->actor.will_collide(toPosition)) {
                this->actor.toPosition.x = toPosition.x;
                this->actor.toPosition.y = toPosition.y;
                this->actor.moving = true;
            }
            this->actor.movementDelay++;
        } else if ( controller_state->up && controller_state->up >= controller_state->down && controller_state->up >= controller_state->left && controller_state->up >= controller_state->right ) {
            this->actor.direction = Up;
            vector2_t toPosition = {
                this->actor.fromPosition.x,
                floor(this->actor.fromPosition.y / 32) * 32 - 32
            };
            if (this->actor.movementDelay > movementDelayThreshold && !this->actor.will_collide(toPosition)) {
                this->actor.toPosition.x = toPosition.x;
                this->actor.toPosition.y = toPosition.y;
                this->actor.moving = true;
            }
            this->actor.movementDelay++;
        } else if ( controller_state->left && controller_state->left >= controller_state->down && controller_state->left >= controller_state->up && controller_state->left >= controller_state->right ) {
            this->actor.direction = Left;
            vector2_t toPosition = {
                floor(this->actor.fromPosition.x / 32) * 32 - 32,
                this->actor.fromPosition.y
            };
            if (this->actor.movementDelay > movementDelayThreshold && !this->actor.will_collide(toPosition)) {
                this->actor.toPosition.x = toPosition.x;
                this->actor.toPosition.y = toPosition.y;
                this->actor.moving = true;
            }
            this->actor.movementDelay++;
        } else if ( controller_state->right && controller_state->right >= controller_state->down && controller_state->right >= controller_state->up && controller_state->right >= controller_state->left ) {
            this->actor.direction = Right;
            vector2_t toPosition = {
                floor(this->actor.fromPosition.x / 32) * 32 + 32,
                this->actor.fromPosition.y
            };
            if (this->actor.movementDelay > movementDelayThreshold && !this->actor.will_collide(toPosition)) {
                this->actor.toPosition.x = toPosition.x;
                this->actor.toPosition.y = toPosition.y;
                this->actor.moving = true;
            }
            this->actor.movementDelay++;
        }
    }
    if (this->actor.moving && this->actor.movementCounter < 8) {
        this->actor.movementCounter++;
        // this->actor.movementDelay = 0;
        vector2_t pos = lerp2( this->actor.getFromPosition(), this->actor.getToPosition(), this->actor.movementCounter / 8.0f);
        this->actor.position.x = pos.x;
        this->actor.position.y = pos.y;
    }

    // Set the FROM position to the current position when we align with the grid
    if (this->actor.moving && this->actor.position.x == this->actor.toPosition.x && this->actor.position.y == this->actor.toPosition.y) {
        this->actor.fromPosition.x = this->actor.toPosition.x;
        this->actor.fromPosition.y = this->actor.toPosition.y;
        this->actor.moving = false;
        this->actor.movementCounter = 0;
    }
}
