#include "match_state.h"

#include "../vdata/game_match_tilemap.h"

#include "../lib/sound.h"
#include "../lib/definitions.h"


void set_score_sprite_data (uint8_t home_score, uint8_t visitant_score) {
    uint8_t h_score = home_score;
    uint8_t v_score = visitant_score;

    if (home_score > 99) {
        h_score = 99;
    }

    if (visitant_score > 99) {
        v_score = 99;
    }

    set_sprite_tile(SCORE_GOAL_H_1_SPRITE_INDEX, h_score % 10);
    set_sprite_tile(SCORE_GOAL_H_2_SPRITE_INDEX, h_score / 10);

    if (v_score < 10) {
        set_sprite_tile(SCORE_GOAL_V_1_SPRITE_INDEX, v_score);
        set_sprite_tile(SCORE_GOAL_V_2_SPRITE_INDEX, 0);
    } else {
        set_sprite_tile(SCORE_GOAL_V_1_SPRITE_INDEX, v_score / 10);
        set_sprite_tile(SCORE_GOAL_V_2_SPRITE_INDEX, v_score % 10);
    }
}

void show_score (void) {
    move_sprite(SCORE_GOAL_H_1_SPRITE_INDEX, SCORE_GOAL_H_1_SPRITE_X, SCORE_GOAL_SPRITE_Y);
    move_sprite(SCORE_GOAL_V_1_SPRITE_INDEX, SCORE_GOAL_V_1_SPRITE_X, SCORE_GOAL_SPRITE_Y);

    hide_sprite(SCORE_GOAL_H_2_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_V_2_SPRITE_INDEX);

    if (get_sprite_tile(SCORE_GOAL_H_2_SPRITE_INDEX) != 0) {
        move_sprite(SCORE_GOAL_H_2_SPRITE_INDEX, SCORE_GOAL_H_2_SPRITE_X, SCORE_GOAL_SPRITE_Y);
    }

    if (get_sprite_tile(SCORE_GOAL_V_2_SPRITE_INDEX) != 0) {
        move_sprite(SCORE_GOAL_V_2_SPRITE_INDEX, SCORE_GOAL_V_2_SPRITE_X, SCORE_GOAL_SPRITE_Y);
    }
}

void hide_score (void) {
    hide_sprite(SCORE_GOAL_H_1_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_H_2_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_V_1_SPRITE_INDEX);
    hide_sprite(SCORE_GOAL_V_2_SPRITE_INDEX);
}

void set_time_sprite_data (time_t seconds) {
    uint8_t sec = seconds % 60;

    set_sprite_tile(TIME_MIN_SPRITE_INDEX, seconds / 60);
    set_sprite_tile(TIME_DIVIDER_SPRITE_INDEX, TIME_DIVIDER_SPRITE_TILESET_INDEX);
    set_sprite_tile(TIME_SEC_1_SPRITE_INDEX, sec / 10);
    set_sprite_tile(TIME_SEC_2_SPRITE_INDEX, sec % 10);
}

void show_time (void) {
    move_sprite(TIME_DIVIDER_SPRITE_INDEX, TIME_DIVIDER_SPRITE_X, TIME_SPRITE_Y);

    if (get_sprite_tile(TIME_MIN_SPRITE_INDEX) != 9) {
        move_sprite(TIME_MIN_SPRITE_INDEX, TIME_MIN_SPRITE_X, TIME_SPRITE_Y);
        move_sprite(TIME_SEC_1_SPRITE_INDEX, TIME_SEC_1_SPRITE_X, TIME_SPRITE_Y);
        move_sprite(TIME_SEC_2_SPRITE_INDEX, TIME_SEC_2_SPRITE_X, TIME_SPRITE_Y);
    }
}

void handle_match_time (MatchState* match_state) {
    if (!match_state->match_started
        || match_state->time == 0
        || match_state->match_mode == MATCH_MODE_3_GOALS 
        || match_state->match_mode == MATCH_MODE_7_GOALS) {
        return;
    }

    time_t current_clock_time = time(NULL);

    if (current_clock_time != match_state->last_clock_time) {
        match_state->time = match_state->time - 1;
        set_time_sprite_data(match_state->time);
    }

    if (match_state->time == 0) {
        match_state->is_match_ended = TRUE;
    }

    match_state->last_clock_time = current_clock_time;
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

void unset_goalposts_sprites_data (void) {
    for (uint8_t i = 0; i < 8; i++) {
        set_sprite_prop(GOALPOST_2_SPRITE_START_INDEX + i, 0);
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
            tile_x_2 = (uint8_t)(GOALPOST_2_SPRITE_X + 8);

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

void set_goal_label (void) {
    for (uint8_t i = 0; i < 4; i++) {
        set_sprite_tile(LABEL_SPRITE_START_INDEX + i, GOAL_LABEL_SPRITE_TITESET_START_INDEX + i);
    }
}

void set_end_label (void) {
    for (uint8_t i = 0; i < 3; i++) {
        set_sprite_tile(LABEL_SPRITE_START_INDEX + i, END_LABEL_SPRITE_TITESET_START_INDEX + i);
    }
}

void show_goal_label (uint8_t time) {
    for (uint8_t i = 0; i < 4; i++) {
        move_sprite(LABEL_SPRITE_START_INDEX + i, 72 + 8 * i, 88 - (time % 24 == i));
    }
}

void show_end_label (uint8_t time) {
    for (uint8_t i = 0; i < 3; i++) {
        move_sprite(LABEL_SPRITE_START_INDEX + i, 76 + 8 * i, 88 - (time % 42 == i));
    }
}

void hide_label (void) {
    for (uint8_t i = 0; i < 4; i++) {
        hide_sprite(LABEL_SPRITE_START_INDEX + i);
    }
}

void fill_bigcastle_stadium (MatchState* match_state) {
    set_bkg_tiles(0, 0, GameMatchTilemapWidth, GameMatchTilemapHeight, GameMatchTilemap);

    set_score_sprite_data(0, 0);
    show_score();

    set_time_sprite_data(match_state->time);
    show_time();

    set_goalposts_sprites_data();
    show_goalposts();
}

bool check_if_match_ended_with_goals_scored (MatchState* match_state) {
    if (match_state->match_mode == MATCH_MODE_3_GOALS) {
        return match_state->player.goals == 3 || match_state->opponent.goals == 3;
    }
    if (match_state->match_mode == MATCH_MODE_7_GOALS) {
        return match_state->player.goals == 7 || match_state->opponent.goals == 7;
    }
    return FALSE;
}

void reinit_match (MatchState* match_state) {
    put_player_on_the_green_carpet(&match_state->player, match_state->player.char_sprite, match_state->player.goals);
    put_player_on_the_green_carpet(&match_state->opponent, match_state->opponent.char_sprite, match_state->opponent.goals);
    center_the_ball(&match_state->ball);
    match_state->time_to_reinit = 255;
    match_state->ball.goal_scored = FALSE;
}

void handle_goal_scored (MatchState* match_state) {
    if (match_state->time_to_reinit == 255) {
        set_score_sprite_data(match_state->player.goals, match_state->opponent.goals);
        if (match_state->player.goals == 10) {
            show_score();
        }
        set_goal_label();
    }
    show_goal_label(match_state->time_to_reinit);
    match_state->time_to_reinit--;
    if (match_state->time_to_reinit == 0) {
        hide_label();
        if (check_if_match_ended_with_goals_scored(match_state)) {
            match_state->time_to_reinit == 255;
            match_state->is_match_ended = TRUE;
        } else {
            reinit_match(match_state);
            match_state->match_started = FALSE;
        }
    }
}

void handle_match_end (MatchState* match_state, uint8_t current_joypad) {
    if (match_state->time_to_reinit == 255) {
        set_mono();
        play_game_over_sound();
        set_end_label();
    }
    show_end_label(match_state->time_to_reinit);
    match_state->time_to_reinit--;
    if (match_state->time_to_reinit == 0) {
        match_state->time_to_reinit = 254;
    }
    if (current_joypad & 0xFFU && match_state->previous_joypad == 0x00U) {
        hide_label();
        hide_score();
        hide_time();
        hide_goalposts();
        unset_goalposts_sprites_data();
        hide_player(&match_state->player);
        hide_player(&match_state->opponent);
        match_state->game_over = TRUE;
    }
}

uint8_t get_home_char (uint8_t* match_chars) {
    return match_chars[0] + (match_chars[1] << 0x02);
}

uint8_t get_visitant_char (uint8_t* match_chars) {
    return match_chars[2] + (match_chars[3] << 2) + 0xF0;
}

void init_match_state (MatchState* match_state, uint8_t match_mode, uint8_t* match_chars, uint8_t player_side) {
    match_state->match_started = FALSE;
    match_state->match_mode = match_mode;

    uint8_t player_char = get_home_char(match_chars);
    uint8_t opponent_char = get_visitant_char(match_chars);
    if (player_side == PLAYER_SIDE_RIGHT) {
        uint8_t tmp = player_char;
        player_char = opponent_char;
        opponent_char = tmp;
    }

    Player player;
    match_state->player = player;
    put_player_on_the_green_carpet(&match_state->player, player_char, 0);

    Player opponent;
    match_state->opponent = opponent;
    put_player_on_the_green_carpet(&match_state->opponent, opponent_char, 0);

    Ball ball;
    ball.player = &match_state->player;
    ball.opponent = &match_state->opponent;
    match_state->ball = ball;
    center_the_ball(&match_state->ball);

    Bot bot;
    match_state->bot = bot;
    init_bot(&match_state->bot, &match_state->opponent);

    match_state->time = 540;
    if (match_mode == MATCH_MODE_1_MIN) {
        match_state->time = 60;
    }
    if (match_mode == MATCH_MODE_3_MIN) {
        match_state->time = 180;
    }

    time_t current_clock_time;
    time(&current_clock_time);
    match_state->last_clock_time = 0;

    match_state->is_match_ended = FALSE;
    match_state->paused = FALSE;
    match_state->previous_joypad = J_A;

    match_state->time_to_reinit = 255;
    match_state->game_over = FALSE;

    fill_bigcastle_stadium(match_state);
    set_stereo();
}

void update_match_state (MatchState* match_state, uint8_t current_joypad) {
    if (current_joypad & J_START && !(match_state->previous_joypad & J_START)) {
        match_state->paused = !match_state->paused;
    }

    if (match_state->is_match_ended) {
        handle_match_end(match_state, current_joypad);
    }

    if (match_state->paused || match_state->is_match_ended) {
        match_state->previous_joypad = current_joypad;
        return;
    }

    if (!match_state->match_started && current_joypad & 0xFFU && match_state->previous_joypad == 0x00U) {
        match_state->match_started = TRUE;
    }

    if (match_state->match_started) {
        roll_the_ball(&match_state->ball);
        if (match_state->ball.goal_scored && !match_state->is_match_ended) {
            handle_goal_scored(match_state);
        } else {
            handle_match_time(match_state);
            update_player_movement(&match_state->player, current_joypad, match_state->previous_joypad);
            uint8_t previous_bot_joypad = match_state->bot.last_command;
            update_player_movement(
                &match_state->opponent,
                get_bot_command(&match_state->bot, &match_state->ball),
                previous_bot_joypad
            );
        }
    }

    match_state->previous_joypad = current_joypad;
}
