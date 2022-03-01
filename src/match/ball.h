#ifndef _BALL_H_
#define _BALL_H_

#include <gb/gb.h>

typedef struct {
    uint8_t x;
    uint8_t y;

    int8_t gravity;
    int8_t x_speed;
    int8_t y_speed;

    int8_t energy_loss;
    int8_t friction;

    uint8_t stadium_width;
    uint8_t stadium_height;
} Ball;

void center_the_ball (Ball* ball);
void roll_the_ball (Ball* ball);

#endif
