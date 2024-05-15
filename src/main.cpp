#include "bn_bg_palettes.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_vector.h"
#include "bn_timer.h"
#include "common_variable_8x8_sprite_font.h"
#include "bn_regular_bg_items_title.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_bg_palettes.h"
#include "bn_regular_bg_actions.h"
#include "bn_regular_bg_builder.h"
#include "bn_regular_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_regular_bg_animate_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"
#include "bn_regular_bg_attributes_hbe_ptr.h"
#include "globals.h"
#include "scenes/scenes.hpp"
#include "scenes/title.cpp"
#include "scenes/demo.cpp"
#include "actors/actor.cpp"
#include "actors/player.cpp"
#include "maps/overworld.h"
#include "bn_sprite_items_player.h"

SceneType scene;
vector2_t screen_size;
vector2f_t screen_relative_position;
Player *player;
map_t *map;

int main()
{
  bn::core::init();

  scene = SCENE_TITLE;
  screen_size = vector2_t { 240, 160 };
  screen_relative_position = vector2f_t { 0.0, 0.0 };
  map = &overworld_map;

  // Create player and player sprite, hide the sprite. This character will exist in every scene.
  bn::sprite_ptr player_sprite = bn::sprite_items::player.create_sprite(0, 0);
  player = new Player(player_sprite);
  player_sprite.set_visible(false);

  // Set the background color to black
  bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));


  while (true) {
    switch (scene)
    {
    case SCENE_TITLE:
      title_screen();
      break;

    case SCENE_TITLE_MENU:
      // TODO: Implement title menu
      // title_menu();
      // For now, we'll forward directly to the demo scene
      scene = SCENE_DEMO;
      break;

    case SCENE_DEMO:
      demo_scene();
      break;
    
    default:
      bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
      text_generator.set_center_alignment();
      bn::vector<bn::sprite_ptr, 32> text_sprites;
      text_generator.generate(0, 0, "Error: Missing Scene, Please Restart", text_sprites);
      while (true) {
        bn::core::update();
      }
      break;
    }
  }

  while (true)
  {
    bn::core::update();
  }
}
