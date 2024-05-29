#include "save.hpp"

void save_game(SaveFiles save_files_to_save) {
    bn::sram::write(save_files_to_save);
}

SaveFiles load_game() {
    SaveFiles save_files_to_load;
    bn::sram::read(save_files_to_load);
    if (save_files_to_load.files[0].player_position_x == 0xFFFFFFFF && save_files_to_load.files[0].player_position_y == 0xFFFFFFFF && save_files_to_load.files[0].player_health == 0xFFFFFFFF && save_files_to_load.files[0].player_max_health == 0xFFFFFFFF) {
        save_files_to_load = SaveFiles();
        save_game(save_files_to_load);
    }
    return save_files_to_load;
}
