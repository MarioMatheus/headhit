#include "bot.h"

int8_t sign (int8_t x) {
    return (x > 0) - (x < 0);
}

uint8_t dumb_bot (Player* self, Ball* ball) {
    int8_t side_factor = 1;
    if (self->char_sprite & 0xF0) {
        side_factor = -1;
    }

    if (
        sign(ball->x.h - self->x.h - 10 * side_factor) == side_factor * -1
        && sign(ball->x.h - self->x.h - 4 * side_factor) == side_factor
        && (
            self->y_speed == 0 && ball->y.h > self->y.h - 4
            || self->y_speed > 0 && ball->y.h > self->y.h - 14 && ball->y.h < self->y.h - 6
        )
    ) {
        return J_A;
    }

    if (
        sign(ball->x.h - self->x.h - 3 * side_factor) == side_factor * -1
        && sign(ball->x.h - self->x.h + 8 * side_factor) == side_factor
        && ball->y.h > self->y.h - 22
        && ball->y.h < self->y.h - 16
    ) {
        return J_B;
    }
    
    bool hovering_over_head = (
        sign(ball->x.h - self->x.h - 14 * side_factor) == side_factor * -1
        && sign(ball->x.h - self->x.h - 4 * side_factor) == side_factor
        && ball->y.h < self->y.h - 14
        && ball->x_speed < 100
    );

    if (!hovering_over_head && ball->x.h < self->x.h + side_factor * 8) {
        return J_LEFT;
    }

    if (!hovering_over_head && ball->x.h > self->x.h + side_factor * 8) {
        return J_RIGHT;
    }

    return 0;
}

uint8_t get_bot_command (Bot* bot, Ball* ball) {
    if (bot->finger_cooldown > 0) {
        bot->finger_cooldown--;
        return bot->last_command;
    }

    uint8_t command = dumb_bot(bot->player, ball);
    bot->last_command = command;
    bot->finger_cooldown = (((uint8_t) rand()) % bot->finger_force) + 1;

    return command;
}

void init_bot (Bot* bot, Player* player) {
    initrand(0x87);
    initrand(randw());

    bot->player = player;

    bot->finger_force = (((uint8_t) rand()) % 3) + 1;
    bot->finger_cooldown = bot->finger_force;
    bot->last_command = 0x00;
}
