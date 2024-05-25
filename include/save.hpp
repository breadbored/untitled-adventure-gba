#ifndef SAVE_H
#define SAVE_H

#include "bn_sram.h"

struct SaveFile {
    int player_position_x;
    int player_position_y;
    int player_health;
    int player_max_health;
};

struct SaveFiles {
    SaveFile files[3];
};

void save_game(SaveFiles save_files_to_save);

SaveFiles load_game();

#endif
