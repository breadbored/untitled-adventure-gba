#include "scenes/title.hpp"

void title_menu_screen()
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    text_generator.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> title_header_text;

    bn::sprite_text_generator text_generator_small(common::variable_8x8_sprite_font);
    text_generator_small.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> file_1;
    bn::vector<bn::sprite_ptr, 32> file_2;
    bn::vector<bn::sprite_ptr, 32> file_3;
    
    // The screen is 240x160, but the bg is forced to be 256x256, so we have to do math to offset it
    bn::regular_bg_ptr ui_title_bg = bn::regular_bg_items::ui_title.create_bg((256 - 240) / 2, (256 - 160) / 2); 

    text_generator.generate(0, -48, "File Select", title_header_text);

    int center_box_y = 24;

    text_generator_small.generate(0, center_box_y - 16, "File 1", file_1);
    text_generator_small.generate(0, center_box_y, "File 2", file_2);
    text_generator_small.generate(0, center_box_y + 16, "File 3", file_3);

    uint8_t selected_file = 1;
    bool needs_redraw = true;
    while (true)
    {
        if (bn::keypad::up_pressed())
        {
            if (selected_file > 1)
            {
                selected_file--;
                needs_redraw = true;
            }
        }
        else if (bn::keypad::down_pressed())
        {
            if (selected_file < 3)
            {
                selected_file++;
                needs_redraw = true;
            }
        }
        else if (bn::keypad::a_pressed())
        {
            // Load the file
            save_index = selected_file - 1;
            load_file();
            scene = SCENE_DEMO;
            break;
        }
        if (needs_redraw)
        {
            file_1.clear();
            file_2.clear();
            file_3.clear();

            switch (selected_file)
            {
            case 1:
                text_generator_small.generate(0, center_box_y - 16, "> File 1", file_1);
                text_generator_small.generate(0, center_box_y, "File 2", file_2);
                text_generator_small.generate(0, center_box_y + 16, "File 3", file_3);
                break;
            case 2:
                text_generator_small.generate(0, center_box_y - 16, "File 1", file_1);
                text_generator_small.generate(0, center_box_y, "> File 2", file_2);
                text_generator_small.generate(0, center_box_y + 16, "File 3", file_3);
                break;
            case 3:
                text_generator_small.generate(0, center_box_y - 16, "File 1", file_1);
                text_generator_small.generate(0, center_box_y, "File 2", file_2);
                text_generator_small.generate(0, center_box_y + 16, "> File 3", file_3);
                break;
            default:
                break;
            }
        }

        bn::core::update();
    }
}