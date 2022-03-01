#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <gb/gb.h>
#include <stdbool.h>

typedef struct {
    uint8_t menu_index;
    uint8_t next_menu_index;
    uint8_t previous_menu_index;

    uint8_t option_index;

    uint8_t match_mode;
    uint8_t chars[4];

    uint8_t previous_joypad;
    bool is_match_ready;
} MenuState;

void init_menu_state (MenuState* menu_state);
void update_menu_state (MenuState* menu_state, uint8_t current_joypad);

#endif
