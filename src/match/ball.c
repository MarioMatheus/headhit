#include "ball.h"

#include "../lib/definitions.h"

void set_ball_sprite_data (void) {
    set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX);
}

void move_ball (Ball* ball) {
    move_sprite(BALL_SPRITE_INDEX, ball->x, ball->y);
}

void hide_ball () {
    hide_sprite(BALL_SPRITE_INDEX);
}

void center_the_ball (Ball* ball) {
    ball->x = 84;
    ball->y = 88;

    ball->gravity = -1;
    ball->x_speed = 1;
    ball->y_speed = 1;

    ball->energy_loss = 1;
    ball->friction = 1;

    ball->stadium_width = 160;
    ball->stadium_height = 144;

    set_ball_sprite_data();
    move_ball(ball);
}

void roll_the_ball (Ball* ball) {
    move_ball(ball);
}
