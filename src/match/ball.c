#include "ball.h"

#include "../lib/definitions.h"

void set_ball_sprite_data (void) {
    set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX);
}

void move_ball (Ball* ball) {
    move_sprite(BALL_SPRITE_INDEX, ball->x.b.h, ball->y.b.h);
}

void hide_ball () {
    hide_sprite(BALL_SPRITE_INDEX);
}

void center_the_ball (Ball* ball) {
    ball->x.w = 84 << 8;
    ball->y.w = 80 << 8;

    ball->gravity = 8;
    ball->x_speed = 0;
    ball->y_speed = 0;

    ball->energy_loss = 9;
    ball->friction = 1;

    ball->stadium_width = 160;
    ball->stadium_height = BALL_SPRITE_MIN_Y;

    set_ball_sprite_data();
    move_ball(ball);
}

void roll_the_ball (Ball* ball) {
    ball->y.w += ball->y_speed;

    if (ball->y_speed != 0 || ball->y.h != ball->stadium_height) {
        ball->y_speed += ball->gravity;
    }

    if (ball->y.h > ball->stadium_height) {
        ball->y_speed -= ball->gravity;

        uint16_t y_speed_reduced = ball->y_speed / 10 * ball->energy_loss;
        if (y_speed_reduced < 82) {
            ball->y_speed = 0;
            ball->y.h = ball->stadium_height;
        } else {
            ball->y_speed = -y_speed_reduced;
        }
    }

    move_ball(ball);
}