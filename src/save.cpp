#include "save.hpp"

void save_game(SaveFiles save_files_to_save) {
    bn::sram::write(save_files_to_save);
}

SaveFiles load_game() {
    SaveFiles save_files_to_load;
    bn::sram::read(save_files_to_load);
    return save_files_to_load;
}