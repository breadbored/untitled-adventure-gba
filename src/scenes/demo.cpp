#include "scenes/demo.hpp"
#include "globals.h"
#include "maps/map.h"
#include "maps/dungeon_all_directions_map.h"
#include "bn_string.h"
#include "bn_format.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_heart_containers.h"
#include "bn_sprite_items_heart_containers_empty.h"
#include "bn_camera_actions.h"
#include "bn_rect_window.h"
#include "file.hpp"

void scene_init();
void scene_deconstructor();
void scene_draw();

void demo_scene() {
#if DEBUG && DEBUG_PLAYER_COORDINATES
  bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
  text_generator.set_center_alignment();
  bn::vector<bn::sprite_ptr, 32> text_sprites;
#endif

  map = &dungeon_all_directions_map;
  bn::regular_bg_ptr map_bg = map->bg_item.create_bg(0, 0);
  bn::camera_ptr camera = bn::camera_ptr::create(0, 0);
  player->actor.active_sprite.set_camera(camera);
  map_bg.set_camera(camera);
#if DEBUG && DEBUG_COLLISION_MAP
  bn::regular_bg_ptr map_collision_bg = map->bg_collision_item.create_bg(0, 0);
  map_collision_bg.set_camera(camera);
#endif

  scene_init();

  while (true) {
    player->draw();

    camera.set_x(player->actor.position.x);
    camera.set_y(player->actor.position.y);

    scene_draw();

    // Save the game on start press
    // TODO: This should be moved to a menu subscene
    if (bn::keypad::start_pressed()) {
      save_file();
      scene_deconstructor();
      scene = SCENE_TITLE_MENU;
      bn::core::update();
      break;
    }

#if DEBUG && DEBUG_PLAYER_COORDINATES
    text_sprites.clear();
    bn::string<32> text;
    bn::ostringstream text_stream(text);
    text_stream.append(player->actor.getPosition().x);
    text_stream.append(",");
    text_stream.append(player->actor.getPosition().y);
    text_generator.generate(0, -50, text, text_sprites);
#endif

    bn::core::update();
  }
}

bn::optional<bn::sprite_ptr> heart_containers_background[20];
bn::optional<bn::sprite_ptr> heart_containers[20];

void scene_init() {
  // Enable the player sprite
  player->actor.setVisibility(true);
  // Init the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    bn::sprite_ptr heart_container_background = bn::sprite_items::heart_containers_empty.create_sprite(-(screen_size.x / 2) + 16 + (i * 12), -(screen_size.y / 2) + 16);
    bn::sprite_ptr heart_container = bn::sprite_items::heart_containers.create_sprite(-(screen_size.x / 2) + 16 + (i * 12), -(screen_size.y / 2) + 16);
    
    heart_containers_background[i] = heart_container_background;
    heart_containers[i] = heart_container;
  }
}

void scene_deconstructor() {
  // Disable the player sprite
  player->actor.setVisibility(false);
  // Deinit the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    heart_containers[i] = bn::nullopt;
    heart_containers_background[i] = bn::nullopt;
  }
}

void scene_draw() {
  // Draw the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    if (heart_containers[i]) {
      bn::sprite_ptr heart_container = heart_containers[i].value();
      int remaining_health_for_this_heart = player->actor.health - i * 4;
      int frame;
      if (remaining_health_for_this_heart <= 0) {
        frame = 4; // 0 health
      } else if (remaining_health_for_this_heart >= 4) {
        frame = 0; // full health
      } else {
        frame = 4 - remaining_health_for_this_heart; // partial health
      }
      heart_container.set_tiles(bn::sprite_items::heart_containers.tiles_item().create_tiles(frame));
      heart_containers[i]->set_visible(true);
    }
  }

  // Debug the heart containers
  if (bn::keypad::a_pressed()) {
    player->actor.health -= 1;
    if (player->actor.health < 0) {
      player->actor.health = 0;
    }
  }
  if (bn::keypad::b_pressed()) {
    player->actor.health += 1;
    if (player->actor.health > player->actor.max_health) {
      player->actor.health = player->actor.max_health;
    }
  }
}
