#include "scenes/title.hpp"
#include "bn_core.h"
#include "bn_backdrop.h"
#include "bn_span.h"
#include "bn_bg_palette_item.h"
#include "bn_bpp_mode.h"
#include "bn_color.h"

const bn::string<120> text_rows = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z a b c d e f g h i j k l m n o p q r s t u v w x y z - . , ! ? \" # $";
const bn::string<26> text_row_1 = "A B C D E F G H I J K L";
const bn::string<26> text_row_2 = "M N O P Q R S T U V W X";
const bn::string<26> text_row_3 = "Y Z a b c d e f g h i j";
const bn::string<26> text_row_4 = "k l m n o p q r s t u v";
const bn::string<26> text_row_5 = "w x y z - . , ! ? \" # $";

void title_menu_create_file_screen()
{
    bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    text_generator.set_center_alignment();
    bn::vector<bn::sprite_ptr, 32> title_header_text;

    bn::sprite_text_generator text_generator_small(common::fixed_8x8_sprite_font);
    text_generator_small.set_center_alignment();
    bn::vector<bn::sprite_ptr, 4> cursor;
    bn::vector<bn::sprite_ptr, 32> row_1;
    bn::vector<bn::sprite_ptr, 32> row_2;
    bn::vector<bn::sprite_ptr, 32> row_3;
    bn::vector<bn::sprite_ptr, 32> row_4;
    bn::vector<bn::sprite_ptr, 32> row_5;

    // We are ignoring transparency to add another color, this is wiped between scene changes
    bn::bg_palettes::set_transparent_color(bn::color(0x28 * 0.12156862745, 0x33 * 0.12156862745, 0x58 * 0.12156862745));
    // The screen is 240x160, but the bg is forced to be 256x256, so we have to do math to offset it
    bn::regular_bg_ptr ui_title_bg = bn::regular_bg_items::ui_title.create_bg((256 - 240) / 2, (256 - 160) / 2);

    text_generator.generate(0, -48, "Your Name", title_header_text);

    int center_file_box_y = 24;
    bn::string<16> file_name = "";
    bn::ostringstream file_name_stream(file_name);
    int cursor_offset_x = -8;
    int cursor_offset_y = 0;
    int selected_letter_x = 0;
    int selected_letter_y = 0;
    int cursor_x = cursor_offset_x + ((selected_letter_x - 5) * 16);
    int cursor_y = cursor_offset_y + (selected_letter_y * 16);

    text_generator_small.generate(cursor_x, cursor_y, "[ ]", cursor);
    text_generator_small.generate(0, center_file_box_y - 24, text_row_1, row_1);
    text_generator_small.generate(0, center_file_box_y - 12, text_row_2, row_2);
    text_generator_small.generate(0, center_file_box_y, text_row_3, row_3);
    text_generator_small.generate(0, center_file_box_y + 12, text_row_4, row_4);
    text_generator_small.generate(0, center_file_box_y + 24, text_row_5, row_5);

    while (true)
    {
        if (bn::keypad::down_pressed())
        {
            if (selected_letter_y < 4)
            {
                selected_letter_y++;
            }
            else
            {
                selected_letter_y = 0;
            }
        }
        else if (bn::keypad::up_pressed())
        {
            if (selected_letter_y > 0)
            {
                selected_letter_y--;
            }
            else
            {
                selected_letter_y = 4;
            }
        }
        else if (bn::keypad::right_pressed())
        {
            if (selected_letter_x < 11)
            {
                selected_letter_x++;
            }
            else
            {
                selected_letter_x = 0;
            }
        }
        else if (bn::keypad::left_pressed())
        {
            if (selected_letter_x > 0)
            {
                selected_letter_x--;
            }
            else
            {
                selected_letter_x = 11;
            }
        }
        else if (bn::keypad::a_pressed())
        {
            if (!file_name.full())
            {
                // Get index of letter from `text_rows`, compensating for spaces, using selected_letter_x and selected_letter_y
                char letter = text_rows[(selected_letter_x + (selected_letter_y * 12)) * 2];
                file_name_stream.append(letter);
            }
        }
        else if (bn::keypad::b_pressed())
        {
            if (!file_name.empty())
            {
                file_name.pop_back();
            }
            else
            {
                scene = SCENE_TITLE_MENU;
                bn::core::update();
                break;
            }
        }
        else if (bn::keypad::start_pressed())
        {
            if (!file_name.empty())
            {
                for (int i = 0; i < file_name.length(); i++)
                {
                    save_files.files[save_index].file_name[i] = file_name[i];
                }
                for (int i = file_name.length(); i < 16; i++)
                {
                    save_files.files[save_index].file_name[i] = '\0';
                }
                save_files.files[save_index].is_new = false;
                save_game(save_files);
                scene = SCENE_DEMO;
                bn::core::update();
                break;
            }
        }

        cursor_x = cursor_offset_x + ((selected_letter_x - 5) * 16);
        cursor_y = cursor_offset_y + (selected_letter_y * 12);

        cursor.clear();
        text_generator_small.generate(cursor_x, cursor_y, "[ ]", cursor);

        title_header_text.clear();
        if (file_name.length() == 0)
        {
            text_generator.generate(0, -48, "Your Name", title_header_text);
        }
        else
        {
            text_generator.generate(0, -48, file_name, title_header_text);
        }

        bn::core::update();
    }
}