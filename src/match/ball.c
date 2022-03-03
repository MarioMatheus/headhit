#include "ball.h"

#include "../lib/definitions.h"

void set_ball_sprite_data (uint8_t sprite_offset) {
    if (sprite_offset > 3) {
        set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX + sprite_offset - 4);
        set_sprite_prop(BALL_SPRITE_INDEX, S_FLIPX & S_FLIPY);
    } else {
        set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX + sprite_offset);
        set_sprite_prop(BALL_SPRITE_INDEX, 0);
    }
}

void move_ball (Ball* ball) {
    move_sprite(BALL_SPRITE_INDEX, ball->x.b.h, ball->y.b.h);
}

void hide_ball () {
    hide_sprite(BALL_SPRITE_INDEX);
}

void animate_sprite(Ball* ball) {
    if (ball->x_speed > 0 && ball->relative_frame % ball->rotation_divisor == 0) {
        uint8_t sprite_index = get_sprite_tile(BALL_SPRITE_INDEX) - BALL_SPRITE_TITESET_START_INDEX;
        if (ball->is_to_right) {
            sprite_index++;
            if (sprite_index == 8) {
                sprite_index = 0;
            }
        } else {
            if (sprite_index == 0) {
                sprite_index = 7;
            } else {
                sprite_index--;
            }
        }
        set_ball_sprite_data(sprite_index);
    }
}

void center_the_ball (Ball* ball) {
    ball->x.w = 84 << 8;
    ball->y.w = 80 << 8;

    ball->gravity = 9;
    ball->x_speed = 300;
    ball->y_speed = 0;

    ball->relative_frame = 0;
    ball->rotation_divisor = 5;

    ball->is_falling = TRUE;
    ball->is_to_right = TRUE;

    ball->energy_loss = 8;

    ball->stadium_width = 160;
    ball->stadium_height = BALL_SPRITE_MIN_Y;

    set_ball_sprite_data(0);
    move_ball(ball);
}

void roll_the_ball (Ball* ball) {
    if (ball->is_to_right) {
        ball->x.w += ball->x_speed;
    } else {
        if (ball->x.w > ball->x_speed) {
            ball->x.w -= ball->x_speed;
        } else {
            ball->x.w = 0;
        }
    }

    if (ball->is_falling) {
        ball->y.w += ball->y_speed;
    } else {
        ball->y.w -= ball->y_speed;
    }

    if (ball->y_speed != 0 || ball->y.h != ball->stadium_height) {

        if (ball->is_falling) {
            ball->y_speed += ball->gravity;
        } else {
            ball->y_speed -= ball->gravity;
        }

        if (!ball->is_falling && ball->gravity >= ball->y_speed) {
            ball->is_falling = TRUE;
        }
    }


    if (ball->y.h > ball->stadium_height) {
        ball->y_speed -= ball->gravity;

        // Energy loss at y axis
        uint16_t y_speed_reduced = ball->y_speed / 10 * ball->energy_loss;
        fixed next_y;
        next_y.w = ball->y.w - y_speed_reduced;
        if (y_speed_reduced < 10) {
            ball->y_speed = 0;
            ball->y.h = ball->stadium_height;
        } else {
            ball->y_speed = y_speed_reduced;
        }

        ball->is_falling = FALSE;
    }

    if (ball->y.h == ball->stadium_height) {
        // Energy loss at x axis
        uint16_t x_speed_reduced = ball->x_speed / 10 * ball->energy_loss;
        fixed next_x;
        next_x.w = ball->x.w - x_speed_reduced;
        if (x_speed_reduced < 10) {
            ball->x_speed = 0;
            // ball->x.h = ball->stadium_height;
        } else {
            ball->x_speed = x_speed_reduced;
        }

        if (ball->rotation_divisor < 20) {
            ball->rotation_divisor *= 2;
        }
    }

    if (ball->x.h == 0) {
        ball->is_to_right = TRUE;
    }
    
    if (ball->x.h > ball->stadium_width) {
        ball->is_to_right = FALSE;
    }

    animate_sprite(ball);
    move_ball(ball);

    ball->relative_frame++;
    if (ball->relative_frame == 60) {
        ball->relative_frame = 0;
    }
}
