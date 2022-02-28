#include "match_state.h"

#include "../vdata/game_match_tilemap.h"

#include "../lib/sound.h"
#include "../lib/definitions.h"

void fill_bigcastle_stadium (void) {
    set_bkg_tiles(0, 0, GameMatchTilemapWidth, GameMatchTilemapHeight, GameMatchTilemap);

    set_sprite_tile(0, 0);
    set_sprite_tile(1, 0);

    move_sprite(0, 30, 24);
    move_sprite(1, 42, 24);
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
