#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <gb/gb.h>
#include <stdbool.h>

typedef struct {
    uint8_t char_sprite;

    fixed x;
    fixed y;

    uint16_t gravity;
    uint16_t x_speed;
    uint16_t y_speed;

    bool is_falling;
    int8_t movement;

    uint8_t stadium_width;
    uint8_t stadium_height;
} Player;

void put_player_on_the_green_carpet (Player* player, uint8_t char_sprite);
void update_player_movement (Player* player, uint8_t current_joypad);

#endif
