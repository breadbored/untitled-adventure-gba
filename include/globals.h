#ifndef GLOBALS_H
#define GLOBALS_H

#include "scenes/scenes.hpp"
#include "utils.hpp"
#include "actors/player.hpp"
#include "maps/map.h"
#include "save.hpp"

extern SaveFiles save_files;
extern uint8_t save_index;

extern vector2_t screen_size;

extern Player *player;
extern map_t *map;
extern SceneType scene;
extern vector2f_t screen_relative_position;
extern vector2f_t screen_relative_position_prior;

#endif