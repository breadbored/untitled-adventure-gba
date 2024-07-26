#ifndef SAVE_H
#define SAVE_H

#include "bn_sram.h"
#include "bn_string.h"

struct SaveFile {
    alignas(int) char file_name[16];
    bool is_new;
    int player_position_x;
    int player_position_y;
    uint8_t player_health = 4 * 4;
    uint8_t player_max_health = 4 * 4;
};

struct SaveFiles {
    SaveFile files[3] = {
        { "New", true },
        { "New", true },
        { "New", true },
    };
};

void save_game(SaveFiles save_files_to_save);

SaveFiles load_game();

#endif
