#include "scenes/demo.hpp"
#include "globals.h"
#include "maps/map.h"
#include "maps/overworld.h"
#include "bn_string.h"
#include "bn_format.h"
#include "bn_sprite_animate_actions.h"

#define MAX_BUTTON_PRESSES 4

void demo_scene() {
  bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
  text_generator.set_center_alignment();
  bn::vector<bn::sprite_ptr, 32> text_sprites;
  text_generator.generate(0, -50, "Demo Scene", text_sprites);
  bn::vector<bn::sprite_ptr, 32> debug_text;
  

  // bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(0, 0);
  // player_sprite.set_bg_priority(1);

  // Show player sprite
  player->actor.sprite.set_visible(true);

  map = &overworld_map;

  // bn::timer button_timer = bn::timer();
  // int buttonPresses[MAX_BUTTON_PRESSES];
  while (true) {
    player->draw();

    bn::core::update();
  }
}
