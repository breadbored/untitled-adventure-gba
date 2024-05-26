#include "scenes/title.hpp"

void title_menu_screen()
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    text_generator.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> title_header_text;

    bn::sprite_text_generator text_generator_small(common::variable_8x8_sprite_font);
    text_generator_small.set_left_alignment();
    bn::vector<bn::sprite_ptr, 2> cursor;
    bn::vector<bn::sprite_ptr, 32> file_1;
    bn::vector<bn::sprite_ptr, 32> file_2;
    bn::vector<bn::sprite_ptr, 32> file_3;
    
    // We are ignoring transparency to add another color, this is wiped between scene changes
    bn::bg_palettes::set_transparent_color(bn::color(0x28 * 0.12156862745, 0x33 * 0.12156862745, 0x58 * 0.12156862745));
    // The screen is 240x160, but the bg is forced to be 256x256, so we have to do math to offset it
    bn::regular_bg_ptr ui_title_bg = bn::regular_bg_items::ui_title.create_bg((256 - 240) / 2, (256 - 160) / 2); 

    text_generator.generate(0, -48, "File Select", title_header_text);

    int left_file_box_x = -(240 / 2) + 48;
    int center_file_box_y = 24;

    text_generator_small.generate(left_file_box_x, center_file_box_y - 16,">", cursor);
    text_generator_small.generate(left_file_box_x, center_file_box_y - 16, save_files.files[0].file_name, file_1);
    text_generator_small.generate(left_file_box_x, center_file_box_y, save_files.files[1].file_name, file_2);
    text_generator_small.generate(left_file_box_x, center_file_box_y + 16, save_files.files[2].file_name, file_3);

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
            // If the file is new, go to the create file menu. Otherwise, go to the demo scene.
            scene = save_files.files[save_index].is_new ? SCENE_TITLE_CREATE_FILE_MENU : SCENE_DEMO;
            bn::core::update();
            break;
        }
        if (needs_redraw)
        {
            cursor.clear();
            text_generator_small.generate(left_file_box_x - 12, center_file_box_y + ((selected_file - 2) * 16), ">", cursor);
        }

        bn::core::update();
    }
}