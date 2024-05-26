#include "scenes/title.hpp"

void title_screen()
{
  bn::dmg_music::set_master_volume(bn::dmg_music_master_volume::FULL);
  bn::dmg_music_items::title.play();

  bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
  text_generator.set_center_alignment();
  bn::vector<bn::sprite_ptr, 32> studio_name_text;

  bn::timer title_timer = bn::timer();

  // Studio Name
  int studioStartedTime = title_timer.elapsed_ticks();
  text_generator.generate(0, 0, "BreadCodes Presents", studio_name_text);
  while ((title_timer.elapsed_ticks() - studioStartedTime) / 1678 < 60 * 7)
  {
    if (bn::keypad::start_held() && bn::keypad::select_held() && bn::keypad::l_held() && bn::keypad::r_held()) {
      save_files = SaveFiles(); // Overwrite the save files with blank saves // Remove this line to keep the save files
      save_game(save_files);
    }
    bn::core::update();
  }
  // Clear the studio name
  studio_name_text.clear();
  
  // Credits
  bn::vector<bn::sprite_ptr, 32> credits_text;
  int creditsStartedTime = title_timer.elapsed_ticks();
  text_generator.generate(0, -30, "Credits to", credits_text);
  text_generator.generate(0, 0, "Jason C", credits_text);
  text_generator.generate(0, 15, "Nicholas B", credits_text);
  text_generator.generate(0, 30, "Jenna C", credits_text);
  while ((title_timer.elapsed_ticks() - creditsStartedTime) / 1678 < 60 * 7)
  {
    bn::core::update();
  }
  // Clear the credits
  credits_text.clear();

  // Title Background
  bn::regular_bg_ptr title_bg = bn::regular_bg_items::title.create_bg((256 - 240) / 2, (256 - 160) / 2);
  title_bg.set_blending_enabled(true);
  double title_bg_alpha = 0.0;
  bool title_bg_fade_in_complete = false;
  bool ready_to_fade_out = false;
  bool title_bg_fade_out_complete = false;
  // Press Start Text
  bn::vector<bn::sprite_ptr, 32> press_start_text;

  while (true)
  {
    bn::blending::set_transparency_alpha(title_bg_alpha);

    if (!ready_to_fade_out && !title_bg_fade_in_complete)
    {
      if (title_bg_alpha < 1.0)
          title_bg_alpha += 0.01;
      else if (title_bg_alpha > 1.0 && !title_bg_fade_in_complete) {
          text_generator.generate(0, 50, "Press Start", press_start_text);
          title_bg_fade_in_complete = true;
      }
    } else if (ready_to_fade_out && title_bg_fade_in_complete) {
      if (title_bg_alpha > 0.0) {
          title_bg_alpha -= 0.01;
          bn::dmg_music::set_volume(title_bg_alpha);
      }
      else if (title_bg_alpha < 0.0 && !title_bg_fade_out_complete) {
          title_bg_fade_out_complete = true;
          scene = SCENE_TITLE_MENU;
          bn::core::update();
          break;
      }
    }

    if (title_bg_fade_in_complete)
    {
      if(bn::keypad::start_pressed()) {
        ready_to_fade_out = true;
      }
    }

    bn::core::update();
  }

  bn::dmg_music::stop();
}