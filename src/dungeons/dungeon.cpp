#include "globals.h"
#include "utils.hpp"
#include "dungeons/dungeon.hpp"

void DungeonManager::init() {
  this->external_window = bn::rect_window::external();
  this->internal_window = bn::rect_window::internal();

  map = this->dungeon.rooms[this->current_room_index]->map;
  this->p_room_bg = map->bg_item.create_bg(0, 0);
  this->p_camera = bn::camera_ptr::create(0, 0);

  player->actor.active_sprite.set_camera(this->p_camera);
  
  this->p_room_bg.set_camera(this->p_camera);
  this->external_window.set_camera(this->p_camera);
  this->external_window.set_boundaries(
    // Top
    -1024 / 2,
    // Left
    -1024 / 2,
    // Bottom
    1024 / 2,
    // Right
    1024 / 2
  );
  this->external_window.set_show_bg(this->p_room_bg, false);
  this->internal_window.set_camera(this->p_camera);
  this->internal_window.set_boundaries(
    // Top
    -this->p_room_bg.dimensions().height() / 2,
    // Left
    -this->p_room_bg.dimensions().width() / 2,
    // Bottom
    this->p_room_bg.dimensions().height() / 2,
    // Right
    this->p_room_bg.dimensions().width() / 2
  );
  this->internal_window.set_show_all();
  this->p_room_bg.set_blending_enabled(true);
}

void DungeonManager::draw() {
  // Lock the camera to the room
  this->p_camera.set_x((-this->p_room_bg.dimensions().width() / 2) + (240 / 2));
  this->p_camera.set_y((-this->p_room_bg.dimensions().height() / 2) + (160 / 2));

  // If the player reaches the edge of the screen, move to the next room

  // Convert the centered player position to the top left corner
  vector2_t player_aligned_coords = {
    (int)player->actor.position.x + (this->p_room_bg.dimensions().width() / 2),
    (int)player->actor.position.y + (this->p_room_bg.dimensions().height() / 2)
  };
  vector2_t room_coords = this->room_index_to_coords(this->current_room_index);

  // Check if the player is at the edge of the room, if they are passing through a door
  if (!player->actor.animate) {
    if (player_aligned_coords.x >= screen_size.x - 16) {
      // Check if the player is at the right edge of the room
      if (room_coords.x < this->dungeon.width - 1) {
        // Move to the next room
        this->current_room_index = this->coords_to_room_index(room_coords.x + 1, room_coords.y);
        this->redraw_room();

        // Move the player to the left edge of the room
        player->actor.fromPosition.x = (8.0f) - 128.0f;
        player->actor.fromPosition.y = (5.5f * 16.0f) - 4.0f - 128.0f;
        player->actor.toPosition.x = (16.0f + 8.0f) - 128.0f;
        player->actor.toPosition.y = (5.5f * 16.0f) - 4.0f - 128.0f;
        player->actor.animate = true;
        vector2f_t move = { 
          player->actor.toPosition.x - player->actor.fromPosition.x, 
          player->actor.toPosition.y - player->actor.fromPosition.y 
        };
        this->transition_timer = Qsqrt(move.x * move.x + move.y * move.y);
        this->transition_duration = this->transition_timer;
      }
    } else if (player_aligned_coords.x <= 16) {
      // Check if the player is at the left edge of the room
      if (room_coords.x > 0) {
        // Move to the previous room
        this->current_room_index = this->coords_to_room_index(room_coords.x - 1, room_coords.y);
        this->redraw_room();

        // Move the player to the right edge of the room
        player->actor.fromPosition.x = 240.0f - (8.0f) - 128.0f;
        player->actor.fromPosition.y = (5.5f * 16.0f) - 4.0f - 128.0f;
        player->actor.toPosition.x = 240.0f - (16.0f + 8.0f) - 128.0f;
        player->actor.toPosition.y = (5.5f * 16.0f) - 4.0f - 128.0f;
        player->actor.animate = true;
        vector2f_t move = { 
          player->actor.toPosition.x - player->actor.fromPosition.x, 
          player->actor.toPosition.y - player->actor.fromPosition.y 
        };
        this->transition_timer = Qsqrt(move.x * move.x + move.y * move.y);
        this->transition_duration = this->transition_timer;
      }
    } else if (player_aligned_coords.y >= screen_size.y - 16) {
      // Check if the player is at the bottom edge of the room
      if (room_coords.y < this->dungeon.height - 1) {
        // Move to the next room
        this->current_room_index = this->coords_to_room_index(room_coords.x, room_coords.y + 1);
        this->redraw_room();

        // Move the player to the top edge of the room
        player->actor.fromPosition.x = (240.0f / 2.0f) - 128.0f;
        player->actor.fromPosition.y = (16.0f + 8.0f) - 128.0f;
        player->actor.toPosition.x = (240.0f / 2.0f) - 128.0f;
        player->actor.toPosition.y = (32.0f + 8.0f) - 128.0f;
        player->actor.animate = true;
        vector2f_t move = { 
          player->actor.toPosition.x - player->actor.fromPosition.x, 
          player->actor.toPosition.y - player->actor.fromPosition.y 
        };
        this->transition_timer = Qsqrt(move.x * move.x + move.y * move.y);
        this->transition_duration = this->transition_timer;
      }
    } else if (player_aligned_coords.y <= 18) {
      // Check if the player is at the top edge of the room
      if (room_coords.y > 0) {
        // Move to the previous room
        this->current_room_index = this->coords_to_room_index(room_coords.x, room_coords.y - 1);
        this->redraw_room();

        // Move the player to the bottom edge of the room
        player->actor.fromPosition.x = (240.0f / 2.0f) - 128.0f;
        player->actor.fromPosition.y = (160.0f - 8.0f) - 128.0f;
        player->actor.toPosition.x = (240.0f / 2.0f) - 128.0f;
        player->actor.toPosition.y = (160.0f - (16.0f + 8.0f)) - 128.0f;
        player->actor.animate = true;
        vector2f_t move = { 
          player->actor.toPosition.x - player->actor.fromPosition.x, 
          player->actor.toPosition.y - player->actor.fromPosition.y 
        };
        this->transition_timer = Qsqrt(move.x * move.x + move.y * move.y);
        this->transition_duration = this->transition_timer;
      }
    }
  }

  if (this->transition_timer > 0.0f && this->transition_duration > 0.0f) {
    if (!this->p_room_bg.blending_enabled()) this->p_room_bg.set_blending_enabled(true);
    this->transition_timer -= 0.75f;
    bn::fixed progress = this->transition_timer == 0.0f ? 1.0f : 1.0f - (this->transition_timer / this->transition_duration);
    if (progress < 0.5f) {
      // Fade out
      bn::fixed transparency_alpha = 1.0f - (progress * 3.0f);
      bn::fixed clamped = transparency_alpha >= 0.0f ? (transparency_alpha <= 1.0f ? transparency_alpha : 1.0f) : 0.0f;
      bn::blending::set_transparency_alpha(clamped);
    } else if (progress >= 1.0f) {
      // Fade back in entirely, reset
      bn::blending::set_transparency_alpha(1.0f);
    } else if (progress >= 0.5f) {
      // Fade in
      bn::fixed transparency_alpha = (progress - 0.5f) * 2.0f;
      bn::fixed clamped = transparency_alpha >= 0.0f ? (transparency_alpha <= 1.0f ? transparency_alpha : 1.0f) : 0.0f;
      bn::blending::set_transparency_alpha(clamped);
    }
  } else {
    this->transition_timer = 0.0f;
    this->transition_duration = 0.0f;
    if (this->p_room_bg.blending_enabled()) this->p_room_bg.set_blending_enabled(false);
  }
}

void DungeonManager::redraw_room() {
  player->actor.health = this->current_room_index;
  map = this->dungeon.rooms[this->current_room_index]->map;
  this->p_room_bg = map->bg_item.create_bg(0, 0);
  this->p_room_bg.set_camera(this->p_camera);
  this->external_window.set_show_bg(this->p_room_bg, false);
  this->internal_window.set_boundaries(
      // Top
      -this->p_room_bg.dimensions().height() / 2,
      // Left
      -this->p_room_bg.dimensions().width() / 2,
      // Bottom
      this->p_room_bg.dimensions().height() / 2,
      // Right
      this->p_room_bg.dimensions().width() / 2
  );
  this->p_room_bg.set_blending_enabled(true);
}
