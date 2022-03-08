#include "ball.h"

#include "../lib/definitions.h"
#include "../lib/sound.h"

#include "stdio.h"

void set_ball_sprite_data (uint8_t sprite_offset) {
    if (sprite_offset > 3) {
        set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX + sprite_offset - 4);
        set_sprite_prop(BALL_SPRITE_INDEX, S_FLIPX & S_FLIPY);
    } else {
        set_sprite_tile(BALL_SPRITE_INDEX, BALL_SPRITE_TITESET_START_INDEX + sprite_offset);
        set_sprite_prop(BALL_SPRITE_INDEX, 0);
    }
}

void move_ball_sprite (Ball* ball) {
    move_sprite(BALL_SPRITE_INDEX, ball->x.b.h, ball->y.b.h);
}

void hide_ball () {
    hide_sprite(BALL_SPRITE_INDEX);
}

void animate_sprite (Ball* ball) {
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

void move_ball_on_the_x_axis (Ball* ball) {
    if (ball->is_to_right) {
        ball->x.w += ball->x_speed;
    } else {
        if (ball->x.w > ball->x_speed) {
            ball->x.w -= ball->x_speed;
        } else {
            ball->x.w = 0;
        }
    }
}

void move_ball_on_the_y_axis (Ball* ball) {
    if (ball->is_falling) {
        ball->y.w += ball->y_speed;
    } else {
        ball->y.w -= ball->y_speed;
        if (ball->y.h < 10) {
            ball->is_falling = TRUE;
        }
    }
}

void apply_gravity (Ball* ball) {
    if (ball->is_falling) {
        ball->y_speed += ball->gravity;
    } else {
        ball->y_speed -= ball->gravity;
    }

    if (!ball->is_falling && ball->gravity >= ball->y_speed) {
        ball->is_falling = TRUE;
    }
}

void decrease_energy_on_the_y_axis (Ball* ball) {
    uint16_t y_speed_reduced = ball->y_speed / 10 * ball->energy_loss;
    fixed next_y;
    next_y.w = ball->y.w - y_speed_reduced;
    if (y_speed_reduced < 80) {
        ball->y_speed = 0;
        ball->y.h = ball->stadium_height;
    } else {
        ball->y_speed = y_speed_reduced;
    }

    ball->is_falling = FALSE;
}

void decrease_energy_on_the_x_axis (Ball* ball) {
    uint16_t x_speed_reduced = ball->x_speed / 10 * ball->energy_loss;
    fixed next_x;
    next_x.w = ball->x.w - x_speed_reduced;
    if (x_speed_reduced < 10) {
        ball->x_speed = 0;
        // ball->x.h = ball->stadium_height;
    } else {
        ball->x_speed = x_speed_reduced;
    }
}

void apply_matada_no_peito (Ball* ball, Player* player) {
    if (ball->y.h - 4 > player->y.h - 12 && ball->y.h - 4 < player->y.h - 6) {
        ball->energy_loss -= 2;
        decrease_energy_on_the_x_axis(ball);
        ball->energy_loss += 2;
    }
}

void apply_collision_ball_player_by_right (Ball* ball, Player* player) {
    if (
        ball->y.h > player->y.h - 16
        && ball->y.h < player->y.h + 2
        && ball->x.h < player->x.h + 7
        && ball->x.h > player->x.h + 4
    ) {
        ball->is_to_right = TRUE;
        play_bounce_sound(ball->x.h < ball->stadium_width / 2);
        if (!(player->char_sprite & 0xF0) && player->movement < 0) {
            apply_matada_no_peito(ball, player);
        }
    }
}

void apply_collision_ball_player_by_left (Ball* ball, Player* player) {
    if (
        ball->y.h > player->y.h - 16
        && ball->y.h < player->y.h + 2
        && ball->x.h + 8 > player->x.h + 1
        && ball->x.h + 8 < player->x.h + 4
    ) {
        ball->is_to_right = FALSE;
        play_bounce_sound(ball->x.h < ball->stadium_width / 2);
        if (player->char_sprite & 0xF0 && player->movement > 0) {
            apply_matada_no_peito(ball, player);
        }
    }
}

void manage_kick_event_from_player (Ball* ball, Player* player) {
    if (player->j_a_tapped && ball->y.h > player->y.h - 6 && ball->y.h < player->y.h + 2) {
        int8_t kick_force = ball->x.h - player->x.h - 8;

        if (player->char_sprite & 0xF0) {
            kick_force = player->x.h - ball->x.h - 8;
        }

        if (kick_force > -5 && kick_force < 2) {
            ball->x_speed += PLAYER_KICK_FORCE_X / (kick_force + 5);
            ball->y_speed += PLAYER_KICK_FORCE_Y / (kick_force + 5);
            ball->is_to_right = !(player->char_sprite & 0xF0);
        }
    }
}

void increase_rotation_speed (Ball* ball) {
    if (ball->rotation_divisor > 5) {
        ball->rotation_divisor /= 2;
    }
}

void decrease_rotation_speed (Ball* ball) {
    if (ball->rotation_divisor < 20) {
        ball->rotation_divisor *= 2;
    }
}

void increase_relative_frame (Ball* ball) {
    ball->relative_frame++;
    if (ball->relative_frame == 60) {
        ball->relative_frame = 0;
    }
}

void center_the_ball (Ball* ball) {
    ball->x.w = 84 << 8;
    ball->y.w = 80 << 8;

    ball->gravity = 9;
    ball->x_speed = 0;
    ball->y_speed = 0;

    ball->relative_frame = 0;
    ball->rotation_divisor = 5;

    ball->is_falling = TRUE;
    ball->is_to_right = TRUE;

    ball->energy_loss = 8;

    ball->stadium_width = BALL_SPRITE_MAX_X;
    ball->stadium_height = BALL_SPRITE_MIN_Y;

    set_ball_sprite_data(0);
    move_ball_sprite(ball);
}

void roll_the_ball (Ball* ball) {
    move_ball_on_the_x_axis(ball);
    move_ball_on_the_y_axis(ball);

    if (ball->y_speed != 0 || ball->y.h != ball->stadium_height) {
        apply_gravity(ball);
    }

    if (ball->y.h > ball->stadium_height) {
        ball->y_speed -= ball->gravity;
        decrease_energy_on_the_y_axis(ball);
        play_bounce_sound(ball->x.h < ball->stadium_width / 2);
    }

    if (ball->y.h == ball->stadium_height) {
        decrease_energy_on_the_x_axis(ball);
        decrease_rotation_speed(ball);
    }

    if (ball->x.h == 0) {
        ball->is_to_right = TRUE;
        play_bounce_sound(ball->x.h < ball->stadium_width / 2);
    }

    if (ball->x.h > ball->stadium_width) {
        ball->is_to_right = FALSE;
        play_bounce_sound(ball->x.h < ball->stadium_width / 2);
    }

    manage_kick_event_from_player(ball, ball->player);

    apply_collision_ball_player_by_right(ball, ball->player);
    apply_collision_ball_player_by_left(ball, ball->player);

    animate_sprite(ball);
    move_ball_sprite(ball);
    increase_relative_frame(ball);
}
