#ifndef _BALL_H_
#define _BALL_H_

#include <gb/gb.h>
#include <stdbool.h>

#include "player.h"

typedef struct {
    fixed x;
    fixed y;

    fixed previous_y;

    uint16_t gravity;
    uint16_t x_speed;
    uint16_t y_speed;

    uint8_t relative_frame;
    uint8_t rotation_divisor;

    bool is_falling;
    bool is_to_right;

    uint16_t energy_loss;

    uint8_t stadium_width;
    uint8_t stadium_height;

    Player* player;
} Ball;

void center_the_ball (Ball* ball);
void roll_the_ball (Ball* ball);

#endif
