#ifndef _MATCH_STATE_H_
#define _MATCH_STATE_H_

#include <gb/gb.h>
#include <time.h>
#include <stdbool.h>

#include "ball.h"
#include "player.h"

typedef struct {
    bool match_started;
    uint8_t match_mode;

    Ball ball;
    Player player;

    time_t time;
    time_t last_clock_time;

    bool paused;
    uint8_t previous_joypad;
} MatchState;

void init_match_state (MatchState* match_state, uint8_t match_mode);
void update_match_state (MatchState* menu_state, uint8_t current_joypad);

#endif
