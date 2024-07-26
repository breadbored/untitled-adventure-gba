#include "file.hpp"

void load_file() {
    save_files = load_game();
    SaveFile save_file = save_files.files[save_index];
    save_files.files[0].player_position_x = (240.0f / 2.0f) - 128.0f;
    save_files.files[0].player_position_y = (16.0f + 8.0f) - 128.0f;
    save_files.files[1].player_position_x = (240.0f / 2.0f) - 128.0f;
    save_files.files[1].player_position_y = (16.0f + 8.0f) - 128.0f;
    save_files.files[2].player_position_x = (240.0f / 2.0f) - 128.0f;
    save_files.files[2].player_position_y = (16.0f + 8.0f) - 128.0f;
    player->actor.position.x = save_file.player_position_x;
    player->actor.position.y = save_file.player_position_y;
    player->actor.health = save_file.player_health;
    player->actor.max_health = save_file.player_max_health;
}

void save_file() {
    SaveFile save_file = save_files.files[save_index];
    save_file.player_position_x = player->actor.position.x.round_integer();
    save_file.player_position_y = player->actor.position.y.round_integer();
    save_file.player_health = player->actor.health;
    save_file.player_max_health = player->actor.max_health;
    save_files.files[save_index] = save_file;
    save_game(save_files);
}