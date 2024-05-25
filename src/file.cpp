#include "file.hpp"

void load_file() {
    save_files = load_game();
    SaveFile save_file = save_files.files[save_index];
    player->actor.position.x = save_file.actor_x;
    player->actor.position.y = save_file.actor_y;
}

void save_file() {
    SaveFile save_file = save_files.files[save_index];
    save_file.actor_x = player->actor.position.x;
    save_file.actor_y = player->actor.position.y;
    save_files.files[save_index] = save_file;
    save_game(save_files);
}