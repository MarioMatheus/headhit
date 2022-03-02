#ifndef _BALL_H_
#define _BALL_H_

#include <gb/gb.h>
#include <stdbool.h>

typedef struct {
    fixed x;
    fixed y;

    uint16_t gravity;
    uint16_t x_speed;
    uint16_t y_speed;

    bool is_falling;

    uint16_t energy_loss;
    uint16_t friction;

    uint8_t stadium_width;
    uint8_t stadium_height;
} Ball;

void center_the_ball (Ball* ball);
void roll_the_ball (Ball* ball);

#endif
