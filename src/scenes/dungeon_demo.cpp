#include "scenes/demo.hpp"
#include "globals.h"
#include "maps/map.h"
#include "bn_string.h"
#include "bn_format.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_heart_containers.h"
#include "bn_sprite_items_heart_containers_empty.h"
#include "bn_camera_actions.h"
#include "bn_rect_window.h"
#include "file.hpp"
#include "dungeons/dungeon.hpp"
#include "dungeons/dungeon_1.hpp"

void dungeon_demo_scene_init();
void dungeon_demo_scene_deconstructor();
void dungeon_demo_scene_draw();

bn::optional<bn::sprite_ptr> dungeon_demo_heart_containers_background[20];
bn::optional<bn::sprite_ptr> dungeon_demo_heart_containers[20];

void dungeon_demo() {
  DungeonManager dungeon_manager = DungeonManager(dungeon_1);
  dungeon_manager.init();

  dungeon_demo_scene_init();

  while (true) {
    dungeon_manager.draw();

    player->draw();

    dungeon_demo_scene_draw();

    // Save the game on start press
    // TODO: This should be moved to a menu subscene
    if (bn::keypad::start_pressed()) {
      save_file();
      dungeon_demo_scene_deconstructor();
      scene = SCENE_TITLE_MENU;
      bn::core::update();
      break;
    }

    bn::core::update();
  }

  dungeon_demo_scene_deconstructor();
}

void dungeon_demo_scene_init() {
  // Enable the player sprite
  player->actor.setVisibility(true);
  // Init the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    bn::sprite_ptr heart_container_background = bn::sprite_items::heart_containers_empty.create_sprite(-(screen_size.x / 2) + 16 + (i * 12), -(screen_size.y / 2) + 16);
    bn::sprite_ptr heart_container = bn::sprite_items::heart_containers.create_sprite(-(screen_size.x / 2) + 16 + (i * 12), -(screen_size.y / 2) + 16);
    
    dungeon_demo_heart_containers_background[i] = heart_container_background;
    dungeon_demo_heart_containers[i] = heart_container;
  }
}

void dungeon_demo_scene_deconstructor() {
  // Disable the player sprite
  player->actor.setVisibility(false);
  // Deinit the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    dungeon_demo_heart_containers[i] = bn::nullopt;
    dungeon_demo_heart_containers_background[i] = bn::nullopt;
  }
}

void dungeon_demo_scene_draw() {
  // Draw the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    if (dungeon_demo_heart_containers[i]) {
      bn::sprite_ptr heart_container = dungeon_demo_heart_containers[i].value();
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
      dungeon_demo_heart_containers[i]->set_visible(true);
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
