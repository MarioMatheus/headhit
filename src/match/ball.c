#include "ball.h"

#include "collision.h"

#include "../lib/definitions.h"
#include "../lib/sound.h"


void increase_ball_y_speed (Ball* ball, uint16_t speed) {
    ball->y_speed += speed;
    if (ball->y_speed > 500) {
        ball->y_speed = 500;
    }
}

void decrease_ball_y_speed (Ball* ball, uint16_t speed) {
    if (ball->y_speed < speed) {
        ball->y_speed = 0;
    } else {
        ball->y_speed -= speed;
    }
}

void increase_ball_x_speed (Ball* ball, uint16_t speed) {
    ball->x_speed += speed;
    if (ball->x_speed > 500) {
        ball->x_speed = 500;
    }
}

void decrease_ball_x_speed (Ball* ball, uint16_t speed) {
    if (ball->x_speed < speed) {
        ball->x_speed = 0;
    } else {
        ball->x_speed -= speed;
    }
}

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
    ball->previous_y.w = ball->y.w;
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
        increase_ball_y_speed(ball, ball->gravity);
    } else {
        decrease_ball_y_speed(ball, ball->gravity);
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

void apply_chest_trap (Ball* ball) {
    ball->energy_loss -= 4;
    decrease_energy_on_the_x_axis(ball);
    ball->energy_loss += 4;
}

void apply_head_trap (Ball* ball) {
    ball->energy_loss -= 2;
    decrease_energy_on_the_x_axis(ball);
    ball->energy_loss += 2;
}

void apply_goalpost_net_trap (Ball* ball) {
    ball->energy_loss -= 6;
    decrease_energy_on_the_x_axis(ball);
    ball->energy_loss += 6;
}

void apply_goalpost_trap (Ball* ball) {
    ball->energy_loss += 1;
    decrease_energy_on_the_x_axis(ball);
    ball->energy_loss -= 1;
}

/**
 * Check a collision between ball and player
 * @return -1: No collision
 *          0: Foot collision
 *          1: Chest collision
 *          2: Head collision
 */
int8_t process_collision (Ball* ball, Player* player) {
    uint8_t ball_rect[] = {ball->x.h, ball->y.h - 8, 8, 8};
    uint8_t player_rect[] = {player->x.h + 1, player->y.h - 14, 6, 14};

    bool has_collision = check_rect_collision(ball_rect, player_rect);

    if (!player->in_collision_with_ball && has_collision) {
        if (ball->previous_y.h < player->y.h - 12) {
            return 2;
        }
        // if (ball->y.h > player->y.h - 6) {
        //     return 0;
        // }
        if (ball->y.h > player->y.h - 12) {
            return 1;
        }
        // return 2;
        return 0;
    }

    player->in_collision_with_ball = has_collision;

    return -1;
}

void apply_collision_ball_player (Ball* ball, Player* player) {
    int8_t collision = process_collision(ball, player);
    if (collision < 0) {
        return;
    }
    bool is_right_collision = ball->x.h > player->x.h;
    if (collision < 2) {
        ball->is_to_right = is_right_collision;
        if (
            collision == 1
            && (
                player->char_sprite & 0xF0 && player->movement > 0
                || !(player->char_sprite & 0xF0) && player->movement < 0
            )
        ) {
            apply_chest_trap(ball);
        }
    }
    if (collision == 2) {
        ball->is_falling = FALSE;
        if (player->y_speed > 0 && !player->is_falling) {
            increase_ball_y_speed(ball, 20);
        }
        if (ball->x_speed == 0) {
            ball->x_speed = 50;
        }
        if (is_right_collision && !ball->is_to_right) {
            ball->is_to_right = TRUE;
            apply_head_trap(ball);
        }
        if (!is_right_collision && ball->is_to_right) {
            ball->is_to_right = FALSE;
            apply_head_trap(ball);
        }
    }
}

void manage_kick_event_from_player (Ball* ball, Player* player) {
    if (!player->j_a_tapped) {
        return;
    }
    
    bool is_head_hit = player->head_hit && ball->y.h > player->y.h - 14 && ball->y.h <= player->y.h - 6;
    bool is_kick_hit = !player->head_hit && ball->y.h > player->y.h - 6 && ball->y.h < player->y.h + 2;
    
    if (is_head_hit || is_kick_hit) {
        int8_t kick_force = ball->x.h - player->x.h - 8;

        if (player->char_sprite & 0xF0) {
            kick_force = player->x.h - ball->x.h - 8;
        }

        if (kick_force > -5 && kick_force < 2) {
            increase_ball_x_speed(ball, PLAYER_KICK_FORCE_X * (kick_force + 5));
            if (is_kick_hit) {
                increase_ball_y_speed(ball, PLAYER_KICK_FORCE_Y / (kick_force + 5));
            } else {
                increase_ball_y_speed(ball, PLAYER_KICK_FORCE_Y / ball->y.h - (player->y.h - 14));
            }
            ball->is_to_right = !(player->char_sprite & 0xF0);
        }
    }
}

void manage_ball_at_goalposts (Ball* ball) {
    if (
        !ball->goal_scored
        && (ball->x.h < GOALPOST_1_SPRITE_X + 8 || ball->x.h > GOALPOST_2_SPRITE_X - 8)
        && ball->y.h > GOALPOST_SPRITE_Y - 12
        && ball->y.h < GOALPOST_SPRITE_Y + 2
    ) {
        ball->is_to_right = ball->x.h < GOALPOST_1_SPRITE_X + 8;
        increase_ball_x_speed(ball, 20);
        if (ball->is_falling) {
            decrease_ball_y_speed(ball, ball->gravity);
            apply_goalpost_trap(ball);
            ball->is_falling = FALSE;
        }
    }

    if (
        !ball->goal_scored
        && (ball->x.h < GOALPOST_1_SPRITE_X || ball->x.h > GOALPOST_2_SPRITE_X)
        && ball->y.h > GOALPOST_SPRITE_Y + 2
    ) {
        decrease_ball_y_speed(ball, ball->gravity);
        apply_goalpost_net_trap(ball);
        ball->goal_scored = TRUE;
        if (ball->x.h > GOALPOST_2_SPRITE_X - 8) {
            add_score_to_player(ball->player);
        } else {
            add_score_to_player(ball->opponent);
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

    ball->previous_y.w = 0;

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

    ball->goal_scored = FALSE;

    set_ball_sprite_data(0);
    move_ball_sprite(ball);
}

void roll_the_ball (Ball* ball) {
    move_ball_on_the_x_axis(ball);
    move_ball_on_the_y_axis(ball);

    if (ball->y_speed != 0 || ball->y.h != ball->stadium_height) {
        apply_gravity(ball);
    }

    manage_ball_at_goalposts(ball);

    if (ball->y.h > ball->stadium_height) {
        decrease_ball_y_speed(ball, ball->gravity);
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
    manage_kick_event_from_player(ball, ball->opponent);

    apply_collision_ball_player(ball, ball->player);
    apply_collision_ball_player(ball, ball->opponent);

    animate_sprite(ball);
    move_ball_sprite(ball);
    increase_relative_frame(ball);
}
