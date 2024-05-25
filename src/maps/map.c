#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "utils.hpp"
#include "map.h"
#include "bn_core.h"
#include "bn_sprite_ptr.h"

vector2_t get_screen_position(vector2_t position) {
    return (vector2_t) {
        position.x - screen_relative_position.x + ((screen_size.x / 2) - 16),
        position.y - screen_relative_position.y + ((screen_size.y / 2) - 16)
    };
}
vector2_t get_prior_screen_position(vector2_t position) {
    return (vector2_t) {
        position.x - screen_relative_position_prior.x + ((screen_size.x / 2) - 16),
        position.y - screen_relative_position_prior.y + ((screen_size.y / 2) - 16)
    };
}

vector2_t get_map_position(vector2_t position) {
    return (vector2_t) {
        position.x + screen_relative_position.x - ((screen_size.x / 2) - 16),
        position.y + screen_relative_position.y - ((screen_size.y / 2) - 16)
    };
}

int mmin(int a, int b) {
    return a < b ? a : b;
}

int mmax(int a, int b) {
    return a > b ? a : b;
}

int md_comparator(const void *v1, const void *v2)
{
    const map_tile_t *p1 = (map_tile_t *)v1;
    const map_tile_t *p2 = (map_tile_t *)v2;
    if (p1->tile < p2->tile)
        return -1;
    else if (p1->tile > p2->tile)
        return +1;
    else
        return 0;
}

void draw_section(map_t *map, bn::sprite_item **sprite_arr, const int *tilemap, bool rendered_after_player) {
    vector2_t start_position = get_map_position((vector2_t) { -32, -32 });
    vector2_t end_position = get_map_position((vector2_t) { screen_size.x + 32, screen_size.y + 32 });
    vector2_t start_tile = (vector2_t) { start_position.x / 32, start_position.y / 32 };
    vector2_t end_tile = (vector2_t) { end_position.x / 32, end_position.y / 32 };

    map_tile_t tiles[((320 / 32) + 64) * ((240 / 32) + 64)] = { 0 };
    size_t num_tiles = 0;

    // Because rdp_load_texture_stride is expensive and causes a frame drop when called many times, 
    // we only load the texture when it changes.
    // This can be heavily optimized further by presorting an array of the texture index and 
    // vector2_t of the tile coordinates.
    // That could be used to only load a texture once for all tiles of that type.
    int last_tile_texture = NONE;
    for (int x = start_tile.x; x < end_tile.x; x++) {
        for (int y = start_tile.y; y < end_tile.y; y++) {
            vector2_t screen_position = rendered_after_player ? 
                get_prior_screen_position((vector2_t) { x * 32, y * 32 }) :   // Only for layers rendered after the player
                get_screen_position((vector2_t) { x * 32, y * 32 });          // Only for layers rendered before the player

            // If the coord is outside the map, render default grass
            if (x < 0 || y < 0 || x >= map->width || y >= map->height) {
                if (last_tile_texture != GRASS_DARK) {
                    bn::sprite_item *sprite = *(map_tile_texture[GRASS_DARK / (8*8)]);
                    rdp_sync( SYNC_PIPE );
                    rdp_load_texture_stride( 0, 0, MIRROR_DISABLED, sprite, GRASS_DARK % (8*8) );
                    last_tile_texture = GRASS_DARK;
                }
                rdp_draw_sprite( 0, screen_position.x, screen_position.y, MIRROR_DISABLED );
                continue; // Skip everything else
            }

            // If the tile is empty, don't render
            int tile = tilemap[x + y * map->width];
            if (tile == NONE) continue;

            // If the tile is outside the screen, don't render
            if (screen_position.x < -32 || screen_position.x > screen_size.x + 32 || screen_position.y < -32 || screen_position.y > screen_size.y + 32) {
                continue;
            }

            // // Only load textures as often as they change, and only load the texture if it's not already loaded
            // if (last_tile_texture != tile) {
            //     bn::sprite_item *sprite = *(map_tile_texture[tile / (8*8)]);
            //     rdp_sync( SYNC_PIPE );
            //     rdp_load_texture_stride( 0, 0, MIRROR_DISABLED, sprite, tile % (8*8) );
            //     last_tile_texture = tile;
            // }
            // rdp_draw_sprite( 0, screen_position.x, screen_position.y, MIRROR_DISABLED );

            tiles[num_tiles] = (map_tile_t) { tile, screen_position };
            num_tiles++;
        }
    }

    qsort(tiles, num_tiles, sizeof(map_tile_t), md_comparator);
    
    for (int i = 0; i < num_tiles; i++) {
        int tile = tiles[i].tile;
        vector2_t screen_position = tiles[i].position;
        // Only load textures as often as they change, and only load the texture if it's not already loaded
        if (last_tile_texture != tile) {
            bn::sprite_item *sprite = *(map_tile_texture[tile / (8*8)]);
            rdp_sync( SYNC_PIPE );
            rdp_load_texture_stride( 0, 0, MIRROR_DISABLED, sprite, tile % (8*8) );
            last_tile_texture = tile;
        }
        rdp_draw_sprite( 0, screen_position.x, screen_position.y, MIRROR_DISABLED );
    }
}
