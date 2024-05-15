#include "scenes/title.hpp"

void title_screen()
{
  bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
  text_generator.set_center_alignment();
  bn::vector<bn::sprite_ptr, 32> text_sprites;

  bn::timer title_timer = bn::timer();

  int titleStartedTime = title_timer.elapsed_ticks();
  text_generator.generate(0, 0, "BreadCodes Presents", text_sprites);

  while ((title_timer.elapsed_ticks() - titleStartedTime) / 1678 < 60 * 7)
  {
    bn::core::update();
  }

  text_sprites.clear();

  // text_generator.generate(0, 0, "Untitled Adventure", text_sprites);

  bn::regular_bg_ptr title_bg = bn::regular_bg_items::title.create_bg((256 - 240) / 2, (256 - 160) / 2);
  title_bg.set_blending_enabled(true);
  double title_bg_alpha = 0.0;
  bool title_bg_fade_in_complete = false;

  while (true)
  {
    bn::blending::set_transparency_alpha(title_bg_alpha);
    if (title_bg_alpha < 1.0)
        title_bg_alpha += 0.01;
    else if (title_bg_alpha > 1.0 && !title_bg_fade_in_complete) {
        text_generator.generate(0, 50, "Press Start", text_sprites);
        title_bg_fade_in_complete = true;
    }

    if (title_bg_fade_in_complete)
    {
      if(bn::keypad::start_pressed()) {
        scene = SCENE_TITLE_MENU;
        break;
      }
    }

    bn::core::update();
  }
}