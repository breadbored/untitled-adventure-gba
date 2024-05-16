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

  map = &overworld_map;

  while (true) {
    player->draw();

    bn::core::update();
  }
}
