#include "match_state.h"

#include "../vdata/game_match_tilemap.h"

#include "../lib/sound.h"
#include "../lib/definitions.h"


void set_score_sprite_data (void) {
    set_sprite_tile(SCORE_GOAL_H_1_SPRITE_INDEX, 0);
    set_sprite_tile(SCORE_GOAL_H_2_SPRITE_INDEX, 0);
    set_sprite_tile(SCORE_GOAL_V_1_SPRITE_INDEX, 0);
    set_sprite_tile(SCORE_GOAL_V_2_SPRITE_INDEX, 0);
}

void show_score (void) {
    move_sprite(SCORE_GOAL_H_1_SPRITE_INDEX, SCORE_GOAL_H_1_SPRITE_X, SCORE_GOAL_SPRITE_Y);
    move_sprite(SCORE_GOAL_V_1_SPRITE_INDEX, SCORE_GOAL_V_1_SPRITE_X, SCORE_GOAL_SPRITE_Y);
}

void hide_score (void) {
    hide_sprite(SCORE_GOAL_H_1_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_H_2_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_V_1_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_V_2_SPRITE_INDEX);
}

void set_time_sprite_data (void) {
    set_sprite_tile(TIME_MIN_SPRITE_INDEX, 3);
    set_sprite_tile(TIME_DIVIDER_SPRITE_INDEX, TIME_DIVIDER_SPRITE_TILESET_INDEX);
    set_sprite_tile(TIME_SEC_1_SPRITE_INDEX, 0);
    set_sprite_tile(TIME_SEC_2_SPRITE_INDEX, 0);
}

void show_time (void) {
    move_sprite(TIME_MIN_SPRITE_INDEX, TIME_MIN_SPRITE_X, TIME_SPRITE_Y);
    move_sprite(TIME_DIVIDER_SPRITE_INDEX, TIME_DIVIDER_SPRITE_X, TIME_SPRITE_Y);
    move_sprite(TIME_SEC_1_SPRITE_INDEX, TIME_SEC_1_SPRITE_X, TIME_SPRITE_Y);
    move_sprite(TIME_SEC_2_SPRITE_INDEX, TIME_SEC_2_SPRITE_X, TIME_SPRITE_Y);
}

void hide_time (void) {
    hide_sprite(TIME_MIN_SPRITE_INDEX);
    hide_sprite(TIME_DIVIDER_SPRITE_INDEX);
    hide_sprite(TIME_SEC_1_SPRITE_INDEX);
    hide_sprite(TIME_SEC_2_SPRITE_INDEX);
}

void set_goalposts_sprites_data (void) {
    uint8_t tile_index = GOALPOST_SPRITE_TITESET_START_INDEX;

    for (uint8_t i = 0; i < 8; i++) {
        set_sprite_tile(GOALPOST_1_SPRITE_START_INDEX + i, tile_index);
        set_sprite_tile(GOALPOST_2_SPRITE_START_INDEX + i, tile_index);
        set_sprite_prop(GOALPOST_2_SPRITE_START_INDEX + i, S_FLIPX);
        if (i != 1 && i != 5) {
            tile_index = tile_index + 1;
        }
    }

}

void show_goalposts (void) {
    uint8_t tile_x = GOALPOST_1_SPRITE_X;
    uint8_t tile_x_2 = GOALPOST_2_SPRITE_X;

    uint8_t tile_y = GOALPOST_SPRITE_Y;

    for (uint8_t i = 0; i < 8; i++) {
        move_sprite(GOALPOST_1_SPRITE_START_INDEX + i, tile_x, tile_y);
        move_sprite(GOALPOST_2_SPRITE_START_INDEX + i, tile_x_2, tile_y);
        tile_y = tile_y + 8;
        if (i == 3) {
            tile_x = GOALPOST_1_SPRITE_X - 8;
            tile_x_2 = GOALPOST_2_SPRITE_X + 8;

            tile_y = GOALPOST_SPRITE_Y;
        }
    }
}

void hide_goalposts (void) {
    for (uint8_t i = 0; i < 8; i++) {
        hide_sprite(GOALPOST_1_SPRITE_START_INDEX + i);
        hide_sprite(GOALPOST_2_SPRITE_START_INDEX + i);
    }
}

void fill_bigcastle_stadium (void) {
    set_bkg_tiles(0, 0, GameMatchTilemapWidth, GameMatchTilemapHeight, GameMatchTilemap);

    set_score_sprite_data();
    show_score();

    set_time_sprite_data();
    show_time();

    set_goalposts_sprites_data();
    show_goalposts();
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
