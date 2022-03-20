#ifndef _MATCH_STATE_H_
#define _MATCH_STATE_H_

#include <gb/gb.h>
#include <time.h>
#include <stdbool.h>

#include "ball.h"
#include "player.h"
#include "bot.h"

typedef struct {

    bool match_started;
    uint8_t match_mode;

    Ball ball;
    Player player;
    Player opponent;

    time_t time;
    time_t last_clock_time;

    bool is_match_ended;
    bool paused;
    uint8_t previous_joypad;

    uint8_t time_to_reinit;
    bool game_over;

    BotThinking* get_bot_thinking;
    uint8_t bot_previous_joypad;

} MatchState;

uint8_t get_home_char (uint8_t* match_chars);

void init_match_state (MatchState* match_state, uint8_t match_mode, uint8_t* match_chars);
void update_match_state (MatchState* menu_state, uint8_t current_joypad);

#endif
