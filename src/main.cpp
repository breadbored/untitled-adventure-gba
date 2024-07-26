/**
 * Untitled Adventure
 * Copyright 2024
 * BreadCodes / BreadBored
 * Jason Carpenter
 * 
 * This is the main file for the game. It will handle the main loop and scene switching.
*/


/**
 * This is a compiler flag to enable our own debugging features we want to add.
 * 
 * This is a universal flag to turn on/off all other debugging, 
 *  but should be used in conjunction with other debug flags made for specific purposes.
 *  i.e. DEBUG_COLLISION, DEBUG_MOVEMENT, DEBUG_SHOW_COORDINATES, etc.
 *  If you define additional flags, they must depend on if DEBUG is defined and true. For example:
 * 
 * #if DEBUG && DEBUG_COLLISION
 * // Code here to enable collision debugging
 * #endif
*/
#define DEBUG true

// Other flags can be defined here
// These should all also depend on DEBUG being true
#define DEBUG_SKIP_TITLE_SCREEN true // Skip title screen and go straight to title menu. If you need to delete save data, enable DEBUG_RESET_SAVES
#define DEBUG_RESET_SAVES false // Delete saves on startup. You can instead hold start+select+R-Shoulder+L-Shoulder on the "BreadCodes Presents" screen to wipe saves
#define DEBUG_PLAYER_COORDINATES false // Enable the player coordinate logs in the demo scene
#define DEBUG_COLLISION_MAP false // Enable the collision map in the demo scene
#define DEBUG_COLLISION false // Enable print statements in Actor::check_collision()
#define VOLUME_LOW true // Set the volume to 25% of the default volume

// If DEBUG is defined and true, we can turn on logging in the mGBA emulator
#ifndef DEBUG
  #define DEBUG false
#else
  // If DEBUG is set and true, we can turn on logging in the mGBA emulator
  #if DEBUG
    #define BN_CFG_LOG_ENABLED true
    #define BN_CFG_LOG_BACKEND BN_LOG_BACKEND_MGBA
  #endif
#endif

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
#include "scenes/title_menu.cpp"
#include "scenes/title_create_file_menu.cpp"
#include "scenes/demo.cpp"
#include "actors/actor.cpp"
#include "actors/player.cpp"
#include "bn_sprite_items_player.h"
#include "scenes/dungeon_demo.cpp"
#include "dungeons/dungeon.cpp"

SaveFiles save_files;
uint8_t save_index;

SceneType scene;
vector2_t screen_size;
vector2f_t screen_relative_position;
Player *player;
map_t *map;

int main()
{
  bn::core::init();

  // Hold start+select+R-Shoulder+L-Shoulder on the "BreadCodes Presents" screen if you'd like to wipe all save data
  // That code can be found in the first update loop of title_screen()
  save_files = load_game();
  save_index = 0xFF;

  scene = SCENE_TITLE;
#if DEBUG && DEBUG_SKIP_TITLE_SCREEN
  scene = SCENE_TITLE_MENU;
#endif

#if DEBUG_RESET_SAVES
  save_files = SaveFiles();
  save_game(save_files);
#endif

  screen_size = vector2_t { 240, 160 };
  screen_relative_position = vector2f_t { 0.0, 0.0 };

  // Create player and player sprite, hide the sprite. This character will exist in every scene, so it is a global.
  bn::sprite_item player_sprite = bn::sprite_items::player;
  player = new Player(player_sprite);
  
  while (true) {
    // Undo the transparency color change set by some UI scenes
    bn::bg_palettes::set_transparent_color(bn::color(0x28 * 0.12156862745, 0x33 * 0.12156862745, 0x58 * 0.12156862745));

    // Select the scene to run
    switch (scene)
    {
    case SCENE_TITLE:
      title_screen();
      break;

    case SCENE_TITLE_MENU:
      title_menu_screen();
      break;

    case SCENE_TITLE_CREATE_FILE_MENU:
      title_menu_create_file_screen();
      break;

    case SCENE_DEMO:
      // demo_scene();
      dungeon_demo();
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
