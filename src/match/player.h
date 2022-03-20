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

    uint16_t jump_force;
    bool is_falling;
    int8_t movement;

    uint8_t stadium_width;
    uint8_t stadium_height;

    bool j_a_tapped;
    uint8_t kick_cooldown;
    uint8_t kick_animation;

    bool in_collision_with_ball;

    uint8_t relative_frame;

    uint8_t goals;
} Player;

void hide_player (Player* player);
void add_score_to_player (Player* player);

void put_player_on_the_green_carpet (Player* player, uint8_t char_sprite, uint8_t goals);
void update_player_movement (Player* player, uint8_t current_joypad, uint8_t previous_joypad);

#endif
