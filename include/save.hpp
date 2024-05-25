#ifndef SAVE_H
#define SAVE_H

#include "bn_sram.h"

struct SaveFile {
    int actor_x;
    int actor_y;
};

struct SaveFiles {
    SaveFile files[3];
};

void save_game(SaveFiles save_files_to_save);

SaveFiles load_game();

#endif
