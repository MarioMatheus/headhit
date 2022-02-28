#include "match_state.h"

#include "../vdata/game_match_tilemap.h"

#include "../lib/sound.h"
#include "../lib/definitions.h"


void fill_bigcastle_stadium (void) {
    set_bkg_tiles(0, 0, GameMatchTilemapWidth, GameMatchTilemapHeight, GameMatchTilemap);

    // Score
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 0);
    set_sprite_tile(2, 0);
    set_sprite_tile(3, 0);

    move_sprite(0, 30, 24);
    move_sprite(2, 42, 24);

    // Time
    set_sprite_tile(4, 3);
    set_sprite_tile(5, 36);
    set_sprite_tile(6, 0);
    set_sprite_tile(7, 0);

    move_sprite(4, 137, 24);
    move_sprite(5, 142, 24);
    move_sprite(6, 146, 24);
    move_sprite(7, 152, 24);

    // Goalposts
    set_sprite_tile(8, 37);
    set_sprite_tile(9, 38);
    set_sprite_tile(10, 38);
    set_sprite_tile(11, 39);
    set_sprite_tile(12, 40);
    set_sprite_tile(13, 41);
    set_sprite_tile(14, 41);
    set_sprite_tile(15, 42);

    move_sprite(8, 13, 92);
    move_sprite(9, 13, 100);
    move_sprite(10, 13, 108);
    move_sprite(11, 13, 116);
    move_sprite(12, 5, 92);
    move_sprite(13, 5, 100);
    move_sprite(14, 5, 108);
    move_sprite(15, 5, 116);
}

void init_match_state (MatchState* match_state, uint8_t match_mode) {
    match_state->match_mode = match_mode;

    match_state->paused = 0;
    match_state->previous_joypad = 0;

    fill_bigcastle_stadium();
}

void update_match_state (MatchState* match_state, uint8_t current_joypad) {
    if (current_joypad & J_START && !(match_state->previous_joypad & J_START)) {
        match_state->paused = !match_state->paused;
    }

    if (match_state->paused) {
        return;
    }

    match_state->previous_joypad = current_joypad;
}
