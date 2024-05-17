#include "scenes/demo.hpp"
#include "globals.h"
#include "maps/map.h"
#include "maps/overworld.h"
#include "bn_string.h"
#include "bn_format.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_heart_containers.h"

#define MAX_BUTTON_PRESSES 4

void scene_init();
void scene_draw();

void demo_scene() {
  // bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
  // text_generator.set_center_alignment();
  // bn::vector<bn::sprite_ptr, 32> text_sprites;
  // text_generator.generate(0, -50, "Demo Scene", text_sprites);

  map = &overworld_map;
  scene_init();

  while (true) {
    player->draw();

    scene_draw();

    bn::core::update();
  }
}

bn::optional<bn::sprite_ptr> heart_containers[15];

void scene_init() {
  // Init the health of the player
  for (int i = 0; i < player->actor.max_health / 4; i++) {
    bn::sprite_ptr heart_container = bn::sprite_items::heart_containers.create_sprite(-(screen_size.x / 2) + 16 + (i * 16), (screen_size.y / 2) - 16);
    heart_container.set_visible(false);
    heart_containers[i] = heart_container;
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
