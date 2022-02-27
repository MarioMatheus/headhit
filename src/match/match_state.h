#ifndef _MATCH_STATE_H_
#define _MATCH_STATE_H_

#include <gb/gb.h>

typedef struct {
    uint8_t match_mode;

    uint8_t paused;
    uint8_t previous_joypad;
} MatchState;

void init_match_state (MatchState* match_state, uint8_t match_mode);
void update_match_state (MatchState* menu_state, uint8_t current_joypad);

#endif
